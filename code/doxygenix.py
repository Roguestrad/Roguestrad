#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Copyright 2026 Robert Beckebans

# MIT License

# Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# and associated documentation files (the “Software”), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.

# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


"""
doxygenix.py — Doxygen-XML-supported comment inserter
with *mandatory* Pydantic-AI-Agent for intelligent documentation generation.

Process:
1. Parse Doxygen XML
2. Extract function signature + implementation
3. Pydantic-AI Agent generates final Doxygen comment
4. Comment is inserted in header/CPP (idempotent, no duplicates)
5. Old comment in CPP is deleted
"""

import argparse
import json
import os
import re
import shutil
import subprocess
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, List, Optional, Tuple

import xxhash
from pydantic import BaseModel, Field
from pydantic_ai import (
    Agent,
    ModelMessage,
    ModelMessagesTypeAdapter,
    RunContext,
    UsageLimits,
)
from pydantic_ai.mcp import MCPServerStdio
from pydantic_ai.models.openai import OpenAIChatModel
from pydantic_ai.providers.ollama import OllamaProvider
from pydantic_ai.providers.openai import OpenAIProvider
from pydantic_core import to_jsonable_python
from termcolor import colored
from tqdm import tqdm

STATE_DIR = Path("chats")
STATE_DIR.mkdir(exist_ok=True)

HEADER_EXTS = {".h", ".hpp", ".hh", ".hxx"}
SOURCE_EXTS = {".c", ".cc", ".cpp", ".cxx"}


def is_header(p: Path) -> bool:
    return p.suffix.lower() in HEADER_EXTS


def is_source(p: Path) -> bool:
    return p.suffix.lower() in SOURCE_EXTS


def is_under_dir(path: Path, directory: Optional[Path]) -> bool:
    if directory is None:
        return True
    try:
        path.resolve().relative_to(directory.resolve())
        return True
    except ValueError:
        return False


def should_skip_project_file(path: Path) -> bool:
    name = path.name.lower()
    return name in {"gametypeinfo.h", "gametypeinfo.cpp"}


def default_callsite_roots(project_root: Path) -> List[Path]:
    candidates = [
        project_root / "engine",
        project_root / "games",
        project_root / "shared",
        project_root / "tools",
    ]
    roots = [p for p in candidates if p.exists()]
    return roots or [project_root]


def _looks_like_decl_or_def(line: str, func_name: str) -> bool:
    s = line.strip()
    if not s:
        return False
    if s.startswith("#"):
        return True
    if "typedef" in s or s.startswith("using "):
        return True
    if re.search(rf"\b{re.escape(func_name)}\s*\(", s):
        if re.match(
            r"^\s*(static|extern|inline|ID_INLINE|ID_STATIC|ID_FORCE_INLINE|FORCE_INLINE|const|void|int|float|double|char|long|short|signed|unsigned|struct|class|template)\b",
            line,
        ):
            return True
        if s.endswith(";") and "return" not in s and "=" not in s:
            return True
    return False


def read_file(path: Path) -> List[str]:
    return path.read_text(encoding="utf-8", errors="ignore").splitlines()


def write_file(path: Path, lines: List[str]):
    text = "\n".join(lines)
    if not text.endswith("\n"):
        text += "\n"
    path.write_text(text, encoding="utf-8")


def extract_text(node: ET.Element) -> str:
    return "".join(node.itertext()).strip()


def has_existing_doxygen(lines: List[str], decl_idx: int) -> bool:
    """
    Checks whether a Doxygen comment already exists for the declaration at decl_idx (0-based).

    Supports:
    - /// comments
    - //! comments
    - /** ... */ blocks
    - /*! ... */ blocks
    even when we are currently on the closing line '*/'.
    """
    i = decl_idx - 1
    # Skip empty lines directly above
    while i >= 0 and lines[i].strip() == "":
        i -= 1
    if i < 0:
        return False

    s = lines[i].lstrip()

    # 1) /// chain?
    if s.startswith("///"):
        return True

    # 2) //! Doxygen one-liner
    if s.startswith("//!"):
        return True

    # 3) Start of a Doxygen block?
    if s.startswith("/**") or s.startswith("/*!"):
        return True

    # 4) We might be on '*/' or a middle '* …' line.
    #    In that case, go upward until the block start '/*' and check
    #    if it's a Doxygen block.
    if "*/" in s or s.startswith("*"):
        j = i
        while j >= 0 and "/*" not in lines[j]:
            j -= 1
        if j >= 0:
            start = lines[j].lstrip()
            if start.startswith("/**") or start.startswith("/*!"):
                return True

    return False


def find_decl_anchor(lines: List[str], decl_idx: int) -> int:
    """
    Moves declaration index upwards to include template<...> lines directly above.
    """
    i = decl_idx - 1
    blank_count = 0
    while i >= 0 and lines[i].strip() == "":
        blank_count += 1
        if blank_count > 1:
            return decl_idx
        i -= 1
    if i < 0:
        return decl_idx

    if re.match(r"^\s*template\b", lines[i]):
        return i

    if ">" in lines[i] or "," in lines[i]:
        j = i
        while j >= 0 and lines[j].strip() != "":
            if re.match(r"^\s*template\b", lines[j]):
                return j
            j -= 1

    return decl_idx


def find_class_anchor(lines: List[str], class_line_idx: int) -> int:
    """
    Finds the insertion anchor for a class/struct declaration, including templates
    and leading ID_* macros directly above the class keyword.
    """
    idx = find_decl_anchor(lines, class_line_idx)
    while idx > 0 and lines[idx - 1].strip().startswith("ID_"):
        idx -= 1
    return idx


# -----------------------------------------------------------------------------
# Data Model
# -----------------------------------------------------------------------------
@dataclass
class FuncInfo:
    file: Path
    line: int
    bodyfile: Optional[Path]
    bodystart: Optional[int]
    bodyend: Optional[int]
    name: str
    definition: str
    argsstring: str
    params: List[Tuple[str, Optional[str]]]
    param_docs: Dict[str, str]
    returns: Optional[str]
    return_desc: Optional[str]
    brief: str
    details: Optional[str]
    is_pure_virtual: bool = False


@dataclass
class ClassInfo:
    file: Path
    line: int
    name: str
    kind: str  # 'class' oder 'struct'
    brief: str
    details: Optional[str]
    # Liste der bereits dokumentierten Member: "ReadBits: Reads bits from the buffer."
    member_summaries: List[str]
    # Der rohe Code-Block der Klassendeklaration
    raw_declaration: Optional[str] = None
    original_comment: Optional[str] = None


class ClassCommentModel(BaseModel):
    brief: str = Field(
        description="One concise sentence describing the class's main purpose"
    )
    details: Optional[str] = Field(
        description="Architectural details and intended usage"
    )


def _build_class_context(class_info: ClassInfo) -> str:
    decl = class_info.raw_declaration or "<missing>"
    members = class_info.member_summaries or []
    member_text = "\n".join(f"- {m}" for m in members) if members else "<none>"
    original = class_info.original_comment or "<none>"
    return (
        f"kind: {class_info.kind}\n"
        f"name: {class_info.name}\n"
        f"brief: {class_info.brief or '<none>'}\n"
        f"details: {class_info.details or '<none>'}\n"
        f"declaration:\n{decl}\n"
        f"member_summaries:\n{member_text}\n"
        f"original_comment:\n{original}\n"
    )


def ai_generate_class_comment(
    class_info: ClassInfo,
    llm: str,
    thinking: Optional[object] = None,
    verbose: bool = False,
) -> tuple[ClassCommentModel | None, bool]:
    system_prompt = (
        "You are a senior C++ architect. "
        "You are analyzing source code from Roguestrad, a RBDOOM-3-BFG engine fork which is a derivate of Doom 3 BFG by id Software.\n"
        "I will give you a class/struct declaration and a list of its methods with short descriptions. "
        "Your task is to write an overarching Doxygen documentation for the class. "
        "Focus on design intent and intended usage. "
        "Only mention memory management or ownership if it is explicitly stated in the declaration "
        "or member descriptions.\n"
        "OUTPUT RULES:\n"
        "1) Only fill fields of ClassCommentModel: 'brief' and 'details'.\n"
        "2) Do NOT include comment markers like /*, */, /**, /*! or //.\n"
        "3) Do NOT include Doxygen tags like \\brief, \\class, @brief.\n"
        "4) 'brief' must be a single concise sentence.\n"
        "5) 'details' may be multiple sentences, plain text only.\n"
        "6) If member_summaries is empty, keep 'details' empty unless the declaration explicitly states the purpose.\n"
        "7) If unclear, write 'TODO: clarify ...' instead of guessing.\n"
        "8) Do NOT use Markdown or code fences.\n"
        "9) Do NOT mention thread safety.\n"
        "10) Do NOT infer memory management or ownership without explicit evidence.\n"
        "11) Do NOT mention engine or product names.\n"
    )

    user_prompt = _build_class_context(class_info)

    model = _build_ollama_model_from_llm(llm)
    if thinking is not None:
        agent = Agent(
            model,
            output_type=ClassCommentModel,
            system_prompt=system_prompt,
            model_settings={"thinking": thinking},
        )
    else:
        agent = Agent(model, output_type=ClassCommentModel, system_prompt=system_prompt)

    try:
        result = agent.run_sync(user_prompt=user_prompt)

        if verbose:
            chat_name = class_info.name
            save_history(
                chat_name,
                result.all_messages(),
                f"ai_generate_class_comment for {class_info.name}",
            )

        out = result.output
        if out.brief:
            out.brief = cleanup_text(out.brief)
        if out.details:
            out.details = cleanup_text(out.details)
        if not class_info.member_summaries:
            if out.details and "TODO: clarify" not in out.details:
                out.details = None
        return out, True
    except Exception as e:
        print(f"AI class comment generation error: {e}")
        return None, False


def render_class_comment_block(
    class_info: ClassInfo, model: ClassCommentModel
) -> List[str]:
    brief = model.brief.strip() if model.brief else "TODO: clarify class purpose."
    lines = ["/*!"]
    tag = "\\struct" if class_info.kind == "struct" else "\\class"
    lines.append(f"\t{tag} {class_info.name}")
    lines.append(f"\t\\brief {brief}")
    has_todo = bool(model.details and "TODO" in model.details)
    has_enough_methods = len(class_info.member_summaries) >= 7
    if model.details and not has_todo and has_enough_methods:
        lines.append("")
        for ln in model.details.split("\n"):
            if ln.strip():
                lines.append(f"\t{ln}")
        lines.append("")
    lines.append("*/")
    return lines


def get_func_identifier(func: FuncInfo) -> str:
    """
    Provides a stable identifier for the function, e.g.,
        "idParallelJobList::AddJob"
    from the Doxygen definition + argsstring.

    Fallback is the plain function name ("AddJob").

    Examples:

    definition = "void idParallelJobList::AddJob"
    → ident = "idParallelJobList::AddJob"

    definition = "ID_INLINE uint64 idParallelJobList::GetWaitTimeMicroSec() const"
    → ident = "idParallelJobList::GetWaitTimeMicroSec"

    definition = "void SomeFreeFunc"
    → ident = "SomeFreeFunc"
    """
    sig = (func.definition + func.argsstring).strip()
    if not sig:
        return func.name

    # Take everything before the first parenthesis
    head = sig.split("(", 1)[0]
    head = head.strip()
    head = re.sub(r"\s*::\s*", "::", head)
    head = re.sub(r"\s*<\s*", "<", head)
    head = re.sub(r"\s*>\s*", ">", head)
    head = re.sub(r"\s*,\s*", ",", head)
    if not head:
        return func.name

    # Last token is usually "Class::Func" or "Func"
    parts = head.split()
    ident = parts[-1]

    # If something goes wrong, at least return the plain name
    return ident or func.name


def get_func_display_name(func: FuncInfo) -> str:
    """
    Concise display name for logs:
    - keeps "Class::Func"
    - strips return type, macros, and parameter list
    - handles templates/whitespace inside qualified names
    """
    sig = (func.definition + func.argsstring).strip()
    if not sig:
        return func.name

    head = sig.split("(", 1)[0].strip()
    if not head:
        return func.name

    if func.name:
        pattern = (
            rf"(?P<qual>"
            rf"(?:::)?[A-Za-z_]\w*(?:\s*<[^>]*>)?"
            rf"(?:\s*::\s*[A-Za-z_]\w*(?:\s*<[^>]*>)?)*"
            rf")\s*::\s*{re.escape(func.name)}\s*$"
        )
        m = re.search(pattern, head)
        if m:
            qual = re.sub(r"\s*::\s*", "::", m.group("qual").strip())
            return f"{qual}::{func.name}".strip()

    if func.name and func.name in head:
        last_name_pos = head.rfind(func.name)
        if last_name_pos != -1:
            start = head.rfind(" ", 0, last_name_pos)
            if start == -1:
                return re.sub(r"\s*::\s*", "::", head.strip())
            return re.sub(r"\s*::\s*", "::", head[start + 1 :].strip())

    parts = head.split()
    return parts[-1] if parts else func.name


def _suppress_body_fetch_warning(func_id: str) -> bool:
    prefixes = (
        "gltfItem",
        "gltfAccessor",
        "gltfAnimation",
        "gltfBuffer",
        "gltfBufferView",
        "gltfCamera",
        "gltfData",
        "gltfExtra_",
        "gltfMesh",
        "gltfSampler",
        "gltfSkin",
        "idSwap",
        "type_void",
        "type_scriptevent",
        "type_namespace",
        "type_string",
        "type_float",
        "type_vector",
        "type_entity",
        "type_field",
        "type_function",
        "type_virtualfunction",
        "type_pointer",
        "type_object",
        "type_jumpoffset",
        "type_argsize",
        "type_boolean",
        "As<eventCallback_",
    )
    return func_id.startswith(prefixes)


# -----------------------------------------------------------------------------
# XML Parser
# -----------------------------------------------------------------------------
def parse_doxygen_xml(
    xml_dir: Path,
    repo_root: Path,
    prefer_decl: bool = True,
    collect_classes: bool = False,
) -> List[FuncInfo] | Tuple[List[FuncInfo], List[ClassInfo]]:
    funcs: List[FuncInfo] = []
    classes: List[ClassInfo] = []

    for xf in xml_dir.glob("*.xml"):
        try:
            root = ET.parse(xf).getroot()
        except:
            continue

        if collect_classes and xf.name.lower().startswith(("class", "struct")):
            for cd in root.findall(".//compounddef"):
                kind = (cd.get("kind") or "").strip().lower()
                if kind not in {"class", "struct"}:
                    continue

                cname = cd.findtext("compoundname") or ""
                if not cname:
                    continue

                cbd = cd.find("briefdescription")
                cbrief = extract_text(cbd) if cbd is not None else ""

                cdd = cd.find("detaileddescription")
                cdetails = extract_text(cdd) if cdd is not None else ""
                if cdetails == cbrief:
                    cdetails = ""

                member_summaries: List[str] = []
                for md in cd.findall(".//memberdef[@kind='function']"):
                    mname = md.findtext("name") or ""
                    if not mname:
                        continue
                    mdef = md.findtext("definition") or ""
                    margs = md.findtext("argsstring") or ""
                    sig = (mdef + margs).strip() or mname
                    mbd = md.find("briefdescription")
                    mbrief = extract_text(mbd) if mbd is not None else ""
                    mdd = md.find("detaileddescription")
                    mdetails = extract_text(mdd) if mdd is not None else ""
                    if mdetails == mbrief:
                        mdetails = ""
                    if mbrief:
                        summary = f"{sig}: {mbrief}"
                    else:
                        summary = f"{sig}: TODO: clarify function purpose."
                    if mdetails:
                        summary = f"{summary} Details: {mdetails}"
                    member_summaries.append(summary)

                loc = cd.find("location")
                if loc is None:
                    continue
                cfile = loc.get("file")
                cline = loc.get("line")
                if not cfile or not cline:
                    continue

                class_file = Path(cfile)
                try:
                    class_file = class_file.resolve().relative_to(repo_root.resolve())
                except ValueError:
                    continue

                classes.append(
                    ClassInfo(
                        file=class_file,
                        line=int(cline),
                        name=cname,
                        kind=kind,
                        brief=cbrief,
                        details=cdetails or None,
                        member_summaries=member_summaries,
                        raw_declaration=None,
                    )
                )

        for md in root.findall(".//memberdef[@kind='function']"):
            name = md.findtext("name") or ""
            definition = md.findtext("definition") or ""
            argsstring = md.findtext("argsstring") or ""
            virt_attr = (md.get("virt") or "").lower()
            is_pure_virtual = virt_attr == "pure-virtual"

            # briefdescription
            bd = md.find("briefdescription")
            brief = extract_text(bd) if bd is not None else ""

            dd = md.find("detaileddescription")
            details = extract_text(dd) if dd is not None else ""
            if details == brief:
                details = ""

            # return type
            tnode = md.find("type")
            returns = extract_text(tnode) if tnode is not None else None
            if returns and returns.lower() in ("void", "void*"):
                returns = None

            # params
            param_desc_map: Dict[str, str] = {}
            for pitem in md.findall(".//parameterlist[@kind='param']/parameteritem"):
                pname = pitem.findtext("parameternamelist/parametername") or ""
                pdesc_node = pitem.find("parameterdescription")
                pdesc = extract_text(pdesc_node) if pdesc_node is not None else ""
                if pname and pdesc:
                    param_desc_map[pname] = pdesc

            params = []
            for p in md.findall("param"):
                pname = p.findtext("declname") or ""
                ptype_node = p.find("type")
                ptype = extract_text(ptype_node) if ptype_node is not None else None
                params.append((pname, ptype))

            return_desc = None
            return_node = md.find(".//simplesect[@kind='return']")
            if return_node is None:
                return_node = md.find(".//simplesect[@kind='returns']")
            if return_node is not None:
                return_text = extract_text(return_node)
                if return_text:
                    return_desc = return_text

            # location
            loc = md.find("location")
            if loc is None:
                continue

            file_decl = loc.get("file")
            line_decl = loc.get("line")
            bodyfile = loc.get("bodyfile")
            bodystart = loc.get("bodystart")
            bodyend = loc.get("bodyend")

            if prefer_decl and file_decl and line_decl:
                target_file = Path(file_decl)
                target_line = int(line_decl)
            elif bodyfile and bodystart:
                target_file = Path(bodyfile)
                target_line = int(bodystart)
            else:
                continue

            try:
                target_file = target_file.resolve().relative_to(repo_root.resolve())
            except ValueError:
                continue

            bf = Path(bodyfile) if bodyfile else None
            if bf:
                try:
                    bf = bf.resolve().relative_to(repo_root.resolve())
                except ValueError:
                    continue

            funcs.append(
                FuncInfo(
                    file=target_file,
                    line=target_line,
                    bodyfile=bf,
                    bodystart=int(bodystart) if bodystart else None,
                    bodyend=int(bodyend) if bodyend else None,
                    name=name,
                    definition=definition,
                    argsstring=argsstring,
                    params=params,
                    param_docs=param_desc_map,
                    returns=returns,
                    return_desc=return_desc,
                    brief=brief,
                    details=details,
                    is_pure_virtual=is_pure_virtual,
                )
            )
    if collect_classes:
        return funcs, classes
    return funcs


# -----------------------------------------------------------------------------
# Implementation Extractor
# -----------------------------------------------------------------------------
def extract_implementation(func: FuncInfo, repo_root: Path, max_chars=6000) -> str:
    bodyfile = func.bodyfile or func.file
    if not bodyfile.is_absolute():
        bodyfile = (repo_root / bodyfile).resolve()

    if not bodyfile.exists():
        return ""

    lines = read_file(bodyfile)
    start = (func.bodystart or func.line) - 1
    end = (func.bodyend or func.line) - 1

    start = max(0, min(start, len(lines) - 1))
    end = max(start, min(end, len(lines) - 1))

    excerpt = "\n".join(lines[start : end + 1])
    if len(excerpt) > max_chars:
        excerpt = excerpt[:max_chars] + "\n/* ... truncated ... */"

    return excerpt


def extract_class_declaration_block(
    lines: List[str], class_line_idx: int, max_lines: int = 200
) -> str:
    """
    Extracts only the class/struct declaration header (up to the opening brace).
    Returns an empty string if no declaration header is found within max_lines.
    """
    if not lines or class_line_idx < 0 or class_line_idx >= len(lines):
        return ""

    start_idx = find_class_anchor(lines, class_line_idx)
    end_limit = min(len(lines), start_idx + max_lines)

    collected: List[str] = []

    for i in range(start_idx, end_limit):
        line = lines[i]
        collected.append(line)

        if "{" in line:
            return "\n".join(collected)

        if line.strip().endswith(";"):
            return ""

    return ""


def find_callsite_examples(
    func: FuncInfo,
    func_id: str,
    repo_root: Path,
    search_roots: List[Path],
    max_examples: int = 3,
    context_lines: int = 30,
) -> List[str]:
    if max_examples <= 0:
        return []

    func_name = func.name
    if not func_name:
        return []

    class_name = None
    if "::" in func_id:
        class_name = func_id.rsplit("::", 1)[0]

    patterns = []
    if class_name:
        patterns.append(
            re.compile(rf"\b{re.escape(class_name)}::\s*{re.escape(func_name)}\s*\(")
        )
        patterns.append(re.compile(rf"->\s*{re.escape(func_name)}\s*\("))
        patterns.append(re.compile(rf"\.\s*{re.escape(func_name)}\s*\("))
    patterns.append(re.compile(rf"\b{re.escape(func_name)}\s*\("))

    excluded_dirs = {
        "build",
        "doxygen-xml",
        ".git",
        ".vs",
        ".venv",
        "__pycache__",
        "chats",
        "diags",
        "libs",
    }

    bodyfile = func.bodyfile or func.file
    body_path = (
        (repo_root / bodyfile).resolve()
        if not bodyfile.is_absolute()
        else bodyfile.resolve()
    )
    body_start = func.bodystart or -1
    body_end = func.bodyend or -1

    matches: list[tuple[int, Path, int, list[str]]] = []
    seen: set[tuple[str, int]] = set()

    for root in search_roots:
        if not root.exists():
            continue
        for path in root.rglob("*"):
            if not path.is_file():
                continue
            if path.suffix.lower() not in (HEADER_EXTS | SOURCE_EXTS):
                continue
            if any(part in excluded_dirs for part in path.parts):
                continue

            try:
                rel = path.resolve().relative_to(repo_root.resolve())
            except ValueError:
                rel = path

            lines = read_file(path)
            for idx, line in enumerate(lines):
                if _looks_like_decl_or_def(line, func_name):
                    continue

                matched_priority = None
                for p_idx, pattern in enumerate(patterns):
                    if pattern.search(line):
                        matched_priority = p_idx
                        break

                if matched_priority is None:
                    continue

                # skip matches inside the function's own body
                if (
                    path.resolve() == body_path
                    and body_start != -1
                    and body_end != -1
                    and body_start - 1 <= idx <= body_end - 1
                ):
                    continue

                key = (str(rel), idx)
                if key in seen:
                    continue
                seen.add(key)

                matches.append((matched_priority, rel, idx, lines))

    matches.sort(key=lambda x: (x[0], str(x[1]), x[2]))

    before = max(0, context_lines // 2)
    after = max(0, context_lines - before - 1)

    examples: list[str] = []
    for _prio, rel, idx, lines in matches:
        if len(examples) >= max_examples:
            break
        start = max(0, idx - before)
        end = min(len(lines) - 1, idx + after)
        snippet = "\n".join(lines[start : end + 1])
        examples.append(f"{rel}:{start + 1}-{end + 1}\n{snippet}")

    return examples


def extract_inline_comment(line: str) -> str:
    """
    Extracts a comment that appears on the same line behind code.
    Supports //, ///, //! as well as /* ... */ at the end of the line.
    """
    # prefer Doxygen variants
    for marker in ("///", "//!"):
        pos = line.find(marker)
        if pos != -1:
            return line[pos:].rstrip("\n")

    # normal // comment
    pos = line.find("//")
    if pos != -1:
        return line[pos:].rstrip("\n")

    # simple block comment at the end of line
    pos = line.find("/*")
    if pos != -1:
        return line[pos:].rstrip("\n")

    return ""


def extract_trailing_comment_lines(lines: list[str], idx: int) -> str:
    """
    Collects pure comment lines directly underneath the declaration line idx.
    Stops once something 'real' (code) appears or a block is complete.
    """
    collected: list[str] = []
    i = idx + 1
    started = False

    while i < len(lines):
        line = lines[i].rstrip("\n")
        stripped = line.lstrip()

        if not stripped:
            # Empty lines:
            if started:
                # after start, an empty line separates from the rest
                break
            i += 1
            continue

        # pure comment lines (Doxygen or normal)
        if (
            stripped.startswith("///")
            or stripped.startswith("//!")
            or stripped.startswith("///<")
            or stripped.startswith("//!<")
        ):
            collected.append(line)
            started = True
            i += 1
            continue

        # block comment underneath
        if stripped.startswith("/*") or stripped.startswith("/**"):
            started = True
            collected.append(line)
            i += 1
            # collect the whole block
            while i < len(lines):
                line2 = lines[i].rstrip("\n")
                collected.append(line2)
                if "*/" in line2:
                    i += 1
                    break
                i += 1
            break

        # anything else (code) → stop
        break

        i += 1

    return "\n".join(collected)


def extract_comment_block_above(lines: list[str], idx: int) -> str:
    """
    Collects contiguous comment lines directly above idx (function line).
    Supports //, /* ... */, /** ... */.
    """
    i = idx - 1
    collected: list[str] = []

    in_block = False

    while i >= 0:
        line = lines[i].rstrip("\n")

        # Empty lines between comment and function are still allowed
        if not line.strip():
            if collected:
                # we already have something, an empty line separates from the rest
                break
            i -= 1
            continue

        # Block comments
        if "*/" in line:
            in_block = True
            collected.append(line)
            i -= 1
            continue

        if in_block:
            collected.append(line)
            if "/*" in line:
                # block complete
                break
            i -= 1
            continue

        # Line comments (//)
        stripped = line.lstrip()
        if (
            stripped.startswith("//")
            or stripped.startswith("/*")
            or stripped.startswith("/**")
        ):
            collected.append(line)
            i -= 1
            continue

        # anything else (code) → stop
        break

    collected.reverse()
    return "\n".join(collected)


def extract_comments_for_declaration(lines: list[str], idx: int) -> str:
    """
    Combines:
    - Comment block directly above the function line
    - Inline comment in the function line itself
    Only comments on the same line or above are considered.
    """
    above = extract_comment_block_above(lines, idx)
    inline = extract_inline_comment(lines[idx])

    parts = [p for p in (above, inline) if p and p.strip()]
    return "\n".join(parts)


def _strip_comments(code: str) -> str:
    # Roughly remove line and block comments
    code = re.sub(r"//.*", "", code)
    code = re.sub(r"/\*.*?\*/", "", code, flags=re.S)
    return code


def is_trivial_destructor(func: FuncInfo, impl: str) -> bool:
    if not func.name or not func.name.startswith("~"):
        return False
    if not impl.strip():
        return False

    code = _strip_comments(impl)
    if "= default" in code or "= delete" in code:
        return True
    raw_lines = code.splitlines()

    lines = []
    for ln in raw_lines:
        s = ln.strip()
        if not s:
            continue
        if s in ("{", "}", ";"):
            continue
        if func.name in s and "(" in s and ")" in s:
            continue
        if re.match(r"^[A-Z_][A-Z0-9_]*$", s):
            continue
        lines.append(s)

    return len(lines) == 0


def _looks_like_simple_return_body(func: FuncInfo, impl: str) -> bool:
    """
    Trivial getter, robust even for inline definitions like:
        ID_INLINE const idVec3& idPlane::Normal() const
        {
            return *reinterpret_cast<const idVec3*>( &a );
        }

    Criteria:
    - non-void return value
    - no parameters
    - body contains exactly ONE return statement
    - otherwise only signature/macros/braces, no further logic
    """
    # 1) Must return something
    if func.returns is None:
        return False

    # 2) No parameters -> classic getter
    if any(name for name, _ in func.params):
        return False

    if not impl.strip():
        return False

    code = _strip_comments(impl)
    raw_lines = code.splitlines()

    # Remove empty lines and bare braces/semicolons
    lines = []
    for ln in raw_lines:
        s = ln.strip()
        if not s:
            continue
        if s in ("{", "}", ";"):
            continue
        lines.append(s)

    saw_return = False

    for ln in lines:
        # 1) Return line?
        if "return" in ln:
            # only one return allowed
            if saw_return:
                return False

            # should start at line beginning
            m = re.match(r"^return\s+(.+);?$", ln)
            if not m:
                return False
            expr = m.group(1).strip().rstrip(";").strip()
            if not expr:
                return False

            # no operators/function calls etc.
            # forbidden = set("()+-*/%&|^!?<>,:")
            # if any(ch in forbidden for ch in expr):
            #     return False

            # no whitespaces in expression
            if " " in expr or "\t" in expr:
                return False

            # simple member/this->member expression
            if not re.match(r"^(this->)?[A-Za-z_]\w*(::[A-Za-z_]\w*)*$", expr):
                return False

            saw_return = True
            continue

        # 2) Allowed signature/macro lines
        #    e.g., "ID_INLINE const idVec3& idPlane::Normal() const"
        #    or just "ID_INLINE"
        if func.name in ln and "(" in ln and ")" in ln:
            # looks like function header -> ok
            continue

        if re.match(r"^[A-Z_][A-Z0-9_]*$", ln):
            # pure macro line: ID_INLINE, FORCE_INLINE, etc.
            continue

        # anything else means additional logic -> not a trivial getter
        return False

    return saw_return


def is_trivial_getter_from_signature(func: FuncInfo) -> bool:
    if func.returns is None:
        if func.name == "Clear":
            return True
        if func.name == "Zero":
            return True
        return False
    if any(name for name, _ in func.params) and not func.name == "ToString":
        return False
    if "::" not in func.definition:
        return False

    sig = (func.definition + " " + func.argsstring).strip()
    is_const = " const" in sig or sig.endswith("const")
    if not is_const and not func.name == "ToFloatPtr":
        return False

    name = func.name
    if name == "Num":
        return True
    if name == "Size":
        return True
    if name == "Allocated":
        return True
    if name == "MemoryUsed":
        return True
    if name == "ByteSize":
        return True
    if name.startswith("Get") and len(name) > 3:
        return True
    if name.startswith("Is") and len(name) > 2:
        return True
    if name.startswith("Has") and len(name) > 3:
        return True
    if name.startswith("To") and len(name) > 2:
        return True

    return False


def _looks_like_simple_call_expr(expr: str) -> bool:
    """
    Simple call expression heuristic for forwarders.
    Accepts:
        - free/static/class calls: Foo(...), ns::Foo(...), Type::Foo(...)
        - member calls: obj.Foo(...), obj->Foo(...), this->Foo(...)
    Validates a single outermost call expression while allowing nested args.
    """
    if "(" not in expr or ")" not in expr:
        return False
    expr = expr.strip()
    m = re.match(
        r"^("
        r"(?:::)?[A-Za-z_]\w*(::[A-Za-z_]\w*)*"  # free/static/class (optional leading ::)
        r"|"
        r"(this|[A-Za-z_]\w*)(\s*(->|\.)\s*[A-Za-z_]\w*)+"  # member chain
        r")\s*\((.*)\)$",
        expr,
    )
    if not m:
        return False

    open_idx = expr.find("(")
    if open_idx == -1 or not expr.endswith(")"):
        return False

    depth = 0
    for i, ch in enumerate(expr[open_idx:], start=open_idx):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth == 0 and i != len(expr) - 1:
                return False
        if depth < 0:
            return False

    return depth == 0


def is_trivial_forwarder(func: FuncInfo, impl: str) -> bool:
    """
    Heuristic for simple forwarding functions:
    - non-void return value
    - body contains exactly one return statement
    - otherwise only signature/macros/braces
    """
    if func.returns is None:
        return False

    if not impl.strip():
        return False

    code = _strip_comments(impl)
    raw_lines = code.splitlines()

    # Remove empty lines and bare braces/semicolons
    lines = []
    for ln in raw_lines:
        s = ln.strip()
        if not s:
            continue
        if s in ("{", "}", ";"):
            continue
        lines.append(s)

    saw_return = False

    for ln in lines:
        # 1) Return line?
        if "return" in ln:
            # only one return allowed
            if saw_return:
                return False

            m = re.match(r"^return\s+(.+);?$", ln)
            if not m:
                return False
            expr = m.group(1).strip().rstrip(";").strip()
            if not expr:
                return False
            if not _looks_like_simple_call_expr(expr):
                return False

            saw_return = True
            continue

        # 2) Allowed signature/macro lines
        if func.name in ln and "(" in ln and ")" in ln:
            continue

        if re.match(r"^[A-Z_][A-Z0-9_]*$", ln):
            continue

        # anything else means additional logic -> not a trivial forwarder
        return False

    return saw_return


def is_trivial_getter(func: FuncInfo, impl: str) -> bool:
    if _looks_like_simple_return_body(func, impl):
        return True
    return is_trivial_getter_from_signature(func)


def count_non_template_params(func: FuncInfo) -> int:
    """
    Returns the number of function parameters, excluding template parameters.
    """
    if not func.params:
        return 0

    count = 0
    for name, ptype in func.params:
        name_clean = (name or "").strip()
        ptype_clean = (ptype or "").strip()
        if not name_clean and ptype_clean in ("", "void"):
            continue
        count += 1
    return count


def is_trivial_setter(func: FuncInfo, impl: str) -> bool:
    """
    Heuristics for trivial setters:
    - void return value (or None in FuncInfo)
    - exactly one parameter
    - body consists of exactly one assignment 'member = param;'
    """

    # Return value: None in FuncInfo means "void"
    if func.returns not in (None, "void", "void "):
        return False

    if len(func.params) != 1:
        return False

    pname, _ptype = func.params[0]
    if not pname:
        return False

    if not impl.strip():
        return False

    code = _strip_comments(impl)
    lines = [ln.strip() for ln in code.splitlines()]
    body_lines = [ln for ln in lines if ln and ln not in ("{", "}", ";")]

    if len(body_lines) != 1:
        return False

    line = body_lines[0]
    if not line.endswith(";"):
        return False
    line = line[:-1].strip()

    if "=" not in line:
        return False
    lhs, rhs = [x.strip() for x in line.split("=", 1)]
    if rhs != pname:
        return False

    # LHS should be a simple member expression
    if any(ch in lhs for ch in "()+-*/%&|^!?<>,: "):
        return False

    return True


# -----------------------------------------------------------------------------
# Pydantic AI Comment Generator
# -----------------------------------------------------------------------------
def _build_ollama_model_from_llm(llm: str | None) -> OpenAIChatModel:
    model_name = "gpt-oss:20b"
    if llm:
        normalized = llm.strip()
        if normalized.startswith("ollama/"):
            normalized = normalized[len("ollama/") :]
        if normalized:
            model_name = normalized

    return OpenAIChatModel(
        model_name=model_name,
        provider=OllamaProvider(base_url="http://localhost:11434/v1"),
    )


ollama_model = _build_ollama_model_from_llm("ollama/gpt-oss:20b")

# llama_cpp_model = OpenAIChatModel(
#     model_name="gpt-oss:20b", # does not matter
#     provider=OpenAIProvider(base_url='http://localhost:8080/v1'),
# )

# Build an MCP stdio server wrapper that will communicate with the subprocess
# mcp_cpp_server = MCPServerStdio(
#     command="mcp-cpp-server.exe", args=["--log-file", "logs/mcp_server.log"]
# )
mcp_cpp_server = None


class CommentModel(BaseModel):
    brief: str = "MISSING"
    details: Optional[str] = None
    params: Dict[str, str] = Field(default_factory=dict)
    returns: Optional[str] = None
    throws: Optional[str] = None


class FileSummaryModel(BaseModel):
    file_purpose: List[str] = Field(
        description="What this file declares/defines, as bullet points"
    )
    core_responsibilities: List[str] = Field(
        description="Bulleted list of responsibilities"
    )
    key_types_and_functions: List[str] = Field(
        description="Most important classes, structs, enums, or functions with em-dash descriptions"
    )
    control_flow: List[str] = Field(
        description="How data or logic flows through this component"
    )
    dependencies: List[str] = Field(
        description="External files or systems this header depends on"
    )
    how_it_fits: List[str] = Field(
        description="How this component relates to the rest of the engine architecture, as bullet points"
    )


class HeaderSummaryModel(FileSummaryModel):
    pass


def _find_file_doxygen_block(
    lines: List[str], scan_limit: int = 200
) -> Optional[Tuple[int, int, Optional[str]]]:
    limit = min(len(lines), scan_limit)
    hash_re = re.compile(
        r"(?:doxygenix|archgen):\s*(?:sha256=([0-9a-f]{64})|xxh128=([0-9a-f]{32}))",
        re.IGNORECASE,
    )

    i = 0
    while i < limit:
        raw = lines[i]
        line = raw.lstrip()
        if line.startswith("///") or line.startswith("//!"):
            if "\\file" in line or "@file" in line:
                m = hash_re.search(line)
                found_hash = (m.group(1) or m.group(2)) if m else None
                return (i, i, found_hash)
        if line.startswith("/*!") or line.startswith("/**"):
            start = i
            end = i
            found_file = False
            found_hash: Optional[str] = None
            found_end = False
            while end < limit:
                if "\\file" in lines[end] or "@file" in lines[end]:
                    found_file = True
                m = hash_re.search(lines[end])
                if m:
                    found_hash = m.group(1) or m.group(2)
                if "*/" in lines[end]:
                    found_end = True
                    break
                end += 1
            if found_file and found_end:
                return (start, end, found_hash)
            if not found_end:
                return None
            i = end + 1
            continue
        i += 1
    return None


def _strip_file_doxygen_block(lines: List[str]) -> List[str]:
    block = _find_file_doxygen_block(lines)
    if not block:
        return list(lines)
    start, end, _hash = block
    return lines[:start] + lines[end + 1 :]


def _has_file_doxygen(lines: List[str], scan_limit: int = 140) -> bool:
    return _find_file_doxygen_block(lines, scan_limit) is not None


def _find_gpl_header_end(lines: List[str]) -> int:
    if not lines:
        return 0
    if lines[0].lstrip().startswith("/*"):
        for i in range(len(lines)):
            if "*/" in lines[i]:
                j = i + 1
                while j < len(lines) and lines[j].strip() == "":
                    j += 1
                return j
    return 0


def _render_file_doxygen_block(
    rel_path: Path, summary: FileSummaryModel, file_sha: str
) -> List[str]:
    def _section(title: str, items: List[str]) -> List[str]:
        section_lines = [f"\t\\par {title}"]
        if items:
            section_lines.extend(f"\t- {item}" for item in items)
        else:
            section_lines.append("\t- TODO: clarify details.")
        return section_lines

    brief = (
        summary.file_purpose[0]
        if summary.file_purpose
        else "TODO: clarify file purpose."
    )
    lines = [
        "/*!",
        f"\t\\file {rel_path.as_posix()}",
        f"\t\\brief {brief}",
        f"\t\\note doxygenix: xxh128={file_sha}",
        "",
    ]
    lines.extend(_section("File Purpose", summary.file_purpose))
    lines.append("")
    lines.extend(_section("Core Responsibilities", summary.core_responsibilities))
    lines.append("")
    lines.extend(_section("Key Types and Functions", summary.key_types_and_functions))
    lines.append("")
    lines.extend(_section("Control Flow", summary.control_flow))
    lines.append("")
    lines.extend(_section("Dependencies", summary.dependencies))
    lines.append("")
    lines.extend(_section("How It Fits", summary.how_it_fits))
    lines.append("*/")
    return lines


def _insert_file_doxygen_comment(
    src_file: Path,
    rel_path: Path,
    summary: FileSummaryModel,
    file_sha: str,
) -> bool:
    lines = read_file(src_file)
    existing = _find_file_doxygen_block(lines)
    block_lines = _render_file_doxygen_block(rel_path, summary, file_sha)

    if existing:
        start, end, existing_hash = existing
        if existing_hash == file_sha:
            return False
        lines[start : end + 1] = block_lines
        next_idx = start + len(block_lines)
        if next_idx < len(lines) and lines[next_idx].strip() != "":
            lines.insert(next_idx, "")
        write_file(src_file, lines)
        return True

    insert_at = _find_gpl_header_end(lines)
    new_lines = lines[:insert_at]
    while new_lines and new_lines[-1].strip() == "":
        new_lines.pop()
    if new_lines and new_lines[-1].strip() != "":
        new_lines.append("")
    new_lines.extend(block_lines)
    while insert_at < len(lines) and lines[insert_at].strip() == "":
        insert_at += 1
    if insert_at < len(lines):
        new_lines.append("")
    new_lines.extend(lines[insert_at:])

    write_file(src_file, new_lines)
    return True


def render_ai_block(model: CommentModel, trivial: bool) -> str:
    if trivial:
        return f"//! {model.brief}\n"

    if (
        model.brief
        and not model.details
        and not model.params
        and not model.returns
        and not model.throws
    ):
        return f"//! {model.brief}\n"

    lines = ["/*!"]
    lines.append(f"\t\\brief {model.brief}")

    if model.details:
        lines.append("")
        for ln in model.details.split("\n"):
            lines.append(f"\t{ln}")
        lines.append("")

    for pname, pdesc in model.params.items():
        lines.append(f"\t\\param {pname} {pdesc}")

    if model.returns:
        lines.append(f"\t\\return {model.returns}")
    if model.throws:
        lines.append(f"\t\\throws {model.throws}")

    # Hard guarantee: close block exactly once
    if not lines[-1].strip().endswith("*/"):
        lines.append("*/")

    return "\n".join(lines) + "\n"


def save_history(chat_id: str, messages: list[ModelMessage], title: str):
    p = STATE_DIR / f"{chat_id}.json"
    try:
        as_python_objects = [to_jsonable_python(msg) for msg in messages]
        data = {"chat_id": chat_id, "title": title, "messages": as_python_objects}
        p.write_text(json.dumps(data, ensure_ascii=False, indent=2), encoding="utf-8")
    except IOError as e:
        print(colored(f"Error saving history for chat_id={chat_id}: {e}", "red"))


def mcp_collect_context(
    func: FuncInfo,
    func_id: str,
    decl_file: Path,
    repo_root: Path,
    mcp_toolset,
    max_examples: int = 3,
    timeout_s: int = 20,
) -> str:
    if not mcp_toolset or MCPServerStdio is None:
        return ""

    try:
        loc_file = decl_file
        if not loc_file.is_absolute():
            loc_file = (repo_root / loc_file).resolve()

        location_hint = f"{loc_file}:{func.line}:1"

        system_prompt = (
            "You are a C++ documentation assistant. "
            "Use the MCP C++ tools to gather symbol context for the given function. "
            "Always call get_project_details first, then search_symbols, then analyze_symbol_context. "
            "Return a compact, plain-text summary of: purpose, key behavior, parameters/returns, "
            "usage examples, and relationships (calls/inheritance) when available."
        )

        user_payload = {
            "symbol": func_id or func.name,
            "fallback_symbol": func.name,
            "location_hint": location_hint,
            "max_examples": max_examples,
            "timeout_s": timeout_s,
        }

        mcp_agent = Agent(
            ollama_model,
            toolsets=[mcp_toolset],
            output_type=str,
            system_prompt=system_prompt,
        )

        res = mcp_agent.run_sync(user_prompt=str(user_payload))
        out = getattr(res, "output", None)
        return out if isinstance(out, str) else ""
    except Exception as e:
        print(f"MCP context collection error for {func_id}: {e}")
        return ""


def ai_generate_comment(
    func: FuncInfo,
    impl: str,
    llm: str,
    trivial: bool,
    thinking: Optional[object] = None,
    verbose: bool = False,
    original_comment: str | None = None,
    mcp_context: str | None = None,
    call_examples: List[str] | None = None,
) -> tuple[CommentModel | None, bool]:
    signature = (func.definition + func.argsstring).strip()

    system_prompt = (
        "You are an experienced senior C++ software engineer. "
        "Your job is to fill the fields of CommentModel for a single C++ function.\n"
        "You are analyzing source code from Roguestrad, a RBDOOM-3-BFG engine fork which is a derivate of Doom 3 BFG by id Software.\n"
        "\n"
        "INPUT YOU RECEIVE:\n"
        "- mode: 'full' or 'trivial'\n"
        "- C++ function signature\n"
        "- Declared return type\n"
        "- List of parameters (name + type)\n"
        "- An excerpt of the implementation/body code\n"
        "- Optional call-site examples (file snippets)\n"
        "- Optional MCP tool context summary\n"
        "\n"
        "YOUR OUTPUT MUST FOLLOW THESE RULES STRICTLY:\n"
        "1) You ONLY fill the JSON fields of CommentModel: 'brief', 'details', 'params', "
        "'returns', 'throws'.\n"
        "2) NEVER include any C++ or Doxygen comment delimiters in any field:\n"
        "   - Do NOT use '/*', '*/', '/**', '/*!', '///', '//' anywhere.\n"
        "3) NEVER include Doxygen tags in any field:\n"
        "   - Do NOT write '@brief', '\\brief', '@param', '\\param', '@return', '\\return', etc.\n"
        "   - Just write natural language descriptions.\n"
        "4) 'brief' is a single concise sentence describing what the function does.\n"
        "5) 'details' (optional) may contain multiple sentences/paragraphs of plain text, "
        "but no comment markers and no Doxygen tags.\n"
        "6) 'params' is a map from parameter name to a short description. "
        "Descriptions must NOT start with '\\param' or '@param', only text.\n"
        "7) 'returns' (if non-void) should be a plain description of the return value "
        "without any Doxygen tags.\n"
        "8) Do NOT write placeholders like 'None', 'null', or 'N/A' in any field. "
        "If a field is not applicable, leave it empty.\n"
        "9) If information is unknown or ambiguous (e.g. units, ownership), "
        "you MUST explicitly write 'TODO: clarify ...' instead of guessing.\n"
        "10) Never wrap anything in code fences or Markdown. Output is pure data for CommentModel.\n"
        "11) Never fill returns for a constructor or destructor.\n"
        "12) Only fill throws if throw is called or it contains an assertation.\n"
        "13) If mode == 'trivial':\n"
        "   - Fill ONLY 'brief' with a single clear sentence.\n"
        "   - All other fields must remain empty (defaults).\n"
        "14) If mode == 'full':\n"
        "   - Fill brief and – if sensible – details, params, returns, throws, pre, post, threadsafety.\n"
        "   - No generic phrases, no repetition of the signature in words.\n"
        "15) If you have get an original_comment than consider it as a viable source for your new comment.\n"
        "16) If call_examples are provided, use them to improve accuracy and phrasing of usage, "
        "but do not describe the example code verbatim.\n"
        "17) If mcp_context is provided, use it to improve accuracy. If it conflicts with the implementation, "
        "prefer the implementation and write 'TODO: clarify ...' for ambiguous details.\n"
        "\n"
        "IMPORTANT: The final Doxygen comment block will be rendered by another component. "
        "You ONLY provide raw text content for the fields, NOT full comment blocks.\n"
    )

    user_prompt = {
        "mode": "trivial" if trivial else "full",
        "signature": signature,
        "returns": func.returns or "",
        "params": [{"name": n, "type": t or ""} for n, t in func.params],
        "impl": impl,
        "original_comment": original_comment or "<none>",
        "call_examples": call_examples or [],
        "mcp_context": mcp_context or "<none>",
    }

    model = _build_ollama_model_from_llm(llm)
    # model = llama_cpp_model
    if thinking is not None:
        agent = Agent(
            model,
            output_type=CommentModel,
            system_prompt=system_prompt,
            model_settings={"thinking": thinking},
        )
    else:
        agent = Agent(model, output_type=CommentModel, system_prompt=system_prompt)

    try:
        r = agent.run_sync(user_prompt=str(user_prompt))

        with open("chats/prompt.md", "w", encoding="utf-8") as f:
            user_prompt_str = json.dumps(user_prompt, indent=2, ensure_ascii=False)
            f.write(
                f"# System prompt\n {system_prompt}\n\n # User prompt\n {user_prompt_str} \n\n"
            )

            # with open("chats/prompt.md", "a", encoding="utf-8") as f:
            block = render_ai_block(r.output, False)
            f.write(f"\n\n# Model output\n{block}\n")

        if verbose:
            func_id = get_func_identifier(func)
            chat_name = func_id
            chat_name = re.sub(r"<[^>]*>", "_template_args_", chat_name)
            chat_name = chat_name.replace("::", ".")
            chat_name = re.sub(r'[\\/:*?"<>|]', "_", chat_name).strip()
            save_history(
                chat_name, r.all_messages(), f"ai_generate_comment for {func_id}"
            )

        def _normalize_optional_text(
            value: Optional[str], allow_null: bool = False
        ) -> Optional[str]:
            if value is None:
                return None
            stripped = value.strip()
            if not stripped:
                return None
            lowered = stripped.lower()
            if lowered in {"none", "n/a"}:
                return None
            if lowered == "null" and not allow_null:
                return None
            return stripped

        model_out = r.output
        is_void = (func.returns or "").strip() == "void"
        if is_void:
            model_out.returns = None
        else:
            model_out.returns = _normalize_optional_text(
                model_out.returns, allow_null=True
            )
        model_out.throws = _normalize_optional_text(model_out.throws)
        if model_out.throws and "todo" in model_out.throws.lower():
            model_out.throws = None

        if model_out.params:
            cleaned = {}
            for key, val in model_out.params.items():
                normalized = _normalize_optional_text(val)
                if normalized:
                    cleaned[key] = normalized
            if cleaned:
                ordered = {}
                for pname, _ptype in func.params:
                    if pname in cleaned:
                        ordered[pname] = cleaned.pop(pname)
                for pname in sorted(cleaned.keys()):
                    ordered[pname] = cleaned[pname]
                model_out.params = ordered
            else:
                model_out.params = {}

        if model_out.brief:
            model_out.brief = cleanup_text(model_out.brief)
        if model_out.details:
            model_out.details = cleanup_text(model_out.details)
        if model_out.returns:
            model_out.returns = cleanup_text(model_out.returns)
        if model_out.throws:
            model_out.throws = cleanup_text(model_out.throws)
        if model_out.params:
            model_out.params = {k: cleanup_text(v) for k, v in model_out.params.items()}

        return model_out, True
    except Exception as e:
        print(f"AI comment generation error: {e}")
        return None, False


# -----------------------------------------------------------------------------
# Insert Comment
# -----------------------------------------------------------------------------
def strip_inline_non_doxygen_comment(line: str) -> str:
    """
    Removes simple inline comments on the same line
    that are not Doxygen comments.
    - // ... (but not ///)
    - /* ... */ on the same line (but not /** or /*!)
    """
    s = line

    # First remove inline // (not ///)
    idx = s.find("//")
    if idx != -1:
        # check if it doesn't start with '///'
        prefix = s[:idx]
        if not s.lstrip().startswith("///"):
            s = s[:idx].rstrip()

    # Then simple /* ... */ on the same line (not /** or /*!)
    idx2 = s.find("/*")
    if idx2 != -1:
        after = s[idx2:]
        if "*/" in after:
            # ensure it's not /** or /*!
            if not after.startswith("/**") and not after.startswith("/*!"):
                s = s[:idx2].rstrip()

    return s


def find_declaration_line_for_body(
    lines: list[str],
    start_idx: int,
    func_id: str,  # e.g. "idParallelJobManagerLocal::Init"
    max_search_up: int = 80,
) -> int:
    """
    Finds the implementation line for a function based on its fully qualified name.
    Example: func_id = "idParallelJobManagerLocal::Init"
    """
    if not lines or start_idx < 0 or start_idx >= len(lines):
        return -1

    # Remove parameter list if provided (sometimes func_id = "idClass::Func(int)")
    base_func_id = func_id.split("(")[0].strip()
    base_func_id = re.sub(r"\s*::\s*", "::", base_func_id)
    base_func_id = re.sub(r"\s*<\s*", "<", base_func_id)
    base_func_id = re.sub(r"\s*>\s*", ">", base_func_id)
    base_func_id = re.sub(r"\s*,\s*", ",", base_func_id)

    def _build_qualified_name_pattern(name: str) -> re.Pattern:
        tokens = re.split(r"(::|<|>|,)", name)
        parts = []
        for token in tokens:
            if not token:
                continue
            if token in ("::", "<", ">", ","):
                parts.append(rf"\s*{re.escape(token)}\s*")
            else:
                parts.append(re.escape(token))
        return re.compile("".join(parts))

    base_func_pattern = _build_qualified_name_pattern(base_func_id)

    # 1. Local upward search (almost always sufficient)
    limit = max(start_idx - max_search_up, -1)
    for i in range(start_idx, limit, -1):
        line = lines[i]

        # Must contain the exact name and an opening parenthesis
        if base_func_pattern.search(line) and (
            "(" in line
            or (i + 1 < len(lines) and "(" in lines[i + 1])
            or (i + 2 < len(lines) and "(" in lines[i + 2])
        ):
            stripped = line.strip()

            # Ignore comments
            if stripped.startswith(("//", "/*", "*")):
                continue

            # Ignore forward declarations
            if stripped.endswith(";"):
                continue

            # If { is on this or the next two lines → this is the implementation
            if (
                "{" in line
                or (i + 1 < len(lines) and "{" in lines[i + 1])
                or (i + 2 < len(lines) and "{" in lines[i + 2])
            ):
                return i

            # Or: if it doesn't end with ; → also good
            if not stripped.endswith(";"):
                return i

    # 2. Fallback: scan entire file for EXACT fully qualified name + implementation
    pattern = re.compile(rf"{base_func_pattern.pattern}\s*\([^)]*\)\s*{{")

    for idx, line in enumerate(lines):
        if base_func_pattern.search(line) and (
            "(" in line
            or (idx + 1 < len(lines) and "(" in lines[idx + 1])
            or (idx + 2 < len(lines) and "(" in lines[idx + 2])
        ):
            stripped = line.strip()
            if stripped.startswith(("//", "/*", "*")) or stripped.endswith(";"):
                continue

            # Exact match: name + ( + params + ) + directly {
            if pattern.search(line):
                return idx

            # Or: { is on the next line (often due to const/noexcept/override)
            if idx + 1 < len(lines) and "{" in lines[idx + 1]:
                next_line = lines[idx + 1].strip()
                if not next_line.startswith(("//", "/*", "*")):
                    return idx

            # Or: { is two lines later (e.g. with = default; or attributes)
            if idx + 2 < len(lines) and "{" in lines[idx + 2]:
                return idx

    # 3. Last resort: find the first (and only!) line with the fully qualified name
    #     that is NOT a forward declaration
    for idx, line in enumerate(lines):
        if base_func_pattern.search(line) and (
            "(" in line
            or (idx + 1 < len(lines) and "(" in lines[idx + 1])
            or (idx + 2 < len(lines) and "(" in lines[idx + 2])
        ):
            stripped = line.strip()
            if not stripped.startswith(("//", "/*", "*")) and not stripped.endswith(
                ";"
            ):
                return idx

    # If all else fails: return -1 (better than wrong)
    return -1


def cleanup_comments_above(
    lines: list[str], idx: int, func_id: str, allow_skip_doxygen: bool = False
) -> int:
    """
    Removes contiguous non-Doxygen comment blocks directly above a declaration/definition.

    Handles /* ... */ blocks ATOMICALLY: scans from a closing */ upward to find
    the matching /* opener and decides as a unit whether it is Doxygen or not.

    Patterns removed:
      - contiguous // lines (including separator/banner lines)
      - non-Doxygen /* ... */ blocks
      - banner blocks made of '=', '-', '*'
    Doxygen blocks (/*!, /**, ///, //!) are preserved unless allow_skip_doxygen=True.
    """

    # If already at the top, nothing to do
    if idx <= 0:
        return idx

    def is_doxygen_line_comment(s: str) -> bool:
        t = s.lstrip()
        return t.startswith("///") or t.startswith("//!")

    def _find_block_opener(lines: list[str], end_idx: int) -> int:
        """Given that lines[end_idx] contains or ends with */, scan upward to find
        the line containing the matching /*. Returns the line index, or -1."""
        j = end_idx
        while j >= 0:
            if "/*" in lines[j]:
                return j
            j -= 1
        return -1

    def _is_doxygen_block(lines: list[str], opener_idx: int) -> bool:
        t = lines[opener_idx].lstrip()
        return t.startswith("/**") or t.startswith("/*!")

    def _line_is_inside_block_comment(s: str) -> bool:
        """True for lines that look like block-comment interior: leading * or */."""
        t = s.lstrip()
        return t.startswith("*/") or (
            t.startswith("*") and not t.startswith("*=") and not t.startswith("* =")
        )

    # ------------------------------------------------------------------
    # Phase 1: collect ranges to delete  [delete_start, delete_end]
    # ------------------------------------------------------------------
    # We walk upward from just above idx, identifying comment chunks.
    # Each chunk is either a block comment (/* ... */), a run of // lines,
    # or empty lines.  We stop when we hit code or a Doxygen comment we
    # must not cross.
    # ------------------------------------------------------------------
    delete_ranges: list[tuple[int, int]] = []  # (start, end) inclusive
    i = idx - 1

    # skip blank lines directly above declaration
    while i >= 0 and lines[i].strip() == "":
        i -= 1

    if i < 0:
        return idx

    while i >= 0:
        cur = lines[i].strip()

        # --- empty line: skip over it ---
        if cur == "":
            i -= 1
            continue

        # --- block comment end (line contains or is  */) ---
        if cur.endswith("*/") or _line_is_inside_block_comment(lines[i]):
            # find the closing */ line first (might be the current one or above)
            block_end = i
            # walk up to find the actual */ if we landed on an interior * line
            while block_end >= 0 and "*/" not in lines[block_end]:
                block_end += 1
                if block_end >= idx:
                    break
            if block_end >= idx:
                block_end = i  # safety: don't go past declaration

            opener = _find_block_opener(lines, block_end)
            if opener < 0:
                # malformed block; treat the single line as a comment
                delete_ranges.append((i, i))
                i -= 1
                continue

            if _is_doxygen_block(lines, opener):
                # This is a Doxygen block
                if allow_skip_doxygen:
                    i = opener - 1
                    continue
                else:
                    break  # preserve it; stop scanning
            else:
                # Non-Doxygen block → mark for deletion
                delete_ranges.append((opener, block_end))
                i = opener - 1
                continue

        # --- Doxygen line comment (///, //!) ---
        if is_doxygen_line_comment(lines[i]):
            if allow_skip_doxygen:
                i -= 1
                continue
            else:
                break  # preserve it; stop scanning

        # --- regular // line comment ---
        if cur.startswith("//"):
            # collect the whole contiguous run of // lines
            run_end = i
            while (
                i >= 0
                and lines[i].strip().startswith("//")
                and not is_doxygen_line_comment(lines[i])
            ):
                i -= 1
            run_start = i + 1
            delete_ranges.append((run_start, run_end))
            continue

        # --- anything else is code → stop ---
        break

    # ------------------------------------------------------------------
    # Phase 2: apply deletions bottom-to-top so indices stay valid
    # ------------------------------------------------------------------
    if not delete_ranges:
        return idx

    # sort by start descending
    delete_ranges.sort(key=lambda r: r[0], reverse=True)

    for d_start, d_end in delete_ranges:
        if d_start < 0 or d_end >= idx:
            continue
        count = d_end - d_start + 1
        del lines[d_start : d_end + 1]
        idx -= count

    # trim excess blank lines above to at most one
    j = idx - 1
    blank_count = 0
    while j >= 0 and lines[j].strip() == "":
        blank_count += 1
        j -= 1
    if blank_count > 1:
        del lines[j + 2 : idx]
        idx -= blank_count - 1

    return idx


def remove_doxygen_block_above(lines: List[str], decl_idx: int) -> int:
    """
    Removes a Doxygen comment directly above decl_idx (0-based).
    Supports:
      - ///, //! - chains
      - /** ... */, /*! ... */ - blocks
    Returns the new index of the declaration line.
    """
    i = decl_idx - 1
    if i < 0:
        return decl_idx

    # like has_existing_doxygen: skip empty lines
    while i >= 0 and lines[i].strip() == "":
        i -= 1
    if i < 0:
        return decl_idx

    s = lines[i].lstrip()

    # 1) line-based Doxygen comments (///, //!)
    if s.startswith("///") or s.startswith("//!"):
        end = i
        start = i
        # collect upwards as long as there are also Doxygen line comments
        while start - 1 >= 0:
            prev = lines[start - 1].lstrip()
            if prev.startswith("///") or prev.startswith("//!"):
                start -= 1
            else:
                break
        del lines[start : end + 1]
        removed = end - start + 1
        return decl_idx - removed

    # 2) Block Doxygen – we may be on '*/' or a '*' line
    if "*/" in s or s.startswith("*"):
        end = i
        j = i
        while j >= 0 and "/*" not in lines[j]:
            j -= 1
        if j >= 0:
            start_line = lines[j].lstrip()
            if start_line.startswith("/**") or start_line.startswith("/*!"):
                start = j
                del lines[start : end + 1]
                removed = end - start + 1
                return decl_idx - removed

    # 3) Block Doxygen – we are directly on '/**' or '/*!'
    if s.startswith("/**") or s.startswith("/*!"):
        start = i
        j = i
        while j < len(lines) and "*/" not in lines[j]:
            j += 1
        end = j if j < len(lines) else i
        del lines[start : end + 1]
        removed = end - start + 1
        return decl_idx - removed

    return decl_idx


def _camel_to_words(name: str) -> str:
    # FooBar -> "foo bar"
    s = re.sub(r"([a-z0-9])([A-Z])", r"\1 \2", name)
    return s.lower()


def _replace_ligatures(text: str) -> str:
    ligatures = {
        "ﬀ": "ff",
        "ﬁ": "fi",
        "ﬂ": "fl",
        "ﬃ": "ffi",
        "ﬄ": "ffl",
        "ﬅ": "ft",
        "ﬆ": "st",
        "ꜳ": "aa",
    }
    for search, replace in ligatures.items():
        text = text.replace(search, replace)
    return text


def _replace_special_spaces(text: str) -> str:
    special_spaces = {
        "\u00a0": " ",
        "\u202f": " ",
        "\u2009": " ",
        "\u2002": " ",
        "\u2003": " ",
    }
    for search, replace in special_spaces.items():
        text = text.replace(search, replace)
    return text


def _replace_quotes(text: str) -> str:
    quotes = {
        "„": '"',
        "“": '"',
        "”": '"',
        "‟": '"',
        "«": '"',
        "»": '"',
        "‚": "'",
        "‘": "'",
        "’": "'",
        "‛": "'",
    }
    for search, replace in quotes.items():
        text = text.replace(search, replace)
    return text


def _replace_dashes(text: str) -> str:
    dashes = {
        "—": "-",
        "–": "-",
        "−": "-",
        "‑": "-",
        "‐": "-",
    }
    for search, replace in dashes.items():
        text = text.replace(search, replace)
    return text


def cleanup_text(text: str) -> str:
    text = _replace_ligatures(text)
    text = _replace_special_spaces(text)
    text = _replace_quotes(text)
    text = _replace_dashes(text)
    return text


def _cleanup_summary_model(summary: FileSummaryModel) -> None:
    summary.file_purpose = [cleanup_text(t) for t in summary.file_purpose]
    summary.core_responsibilities = [
        cleanup_text(t) for t in summary.core_responsibilities
    ]
    summary.key_types_and_functions = [
        cleanup_text(t) for t in summary.key_types_and_functions
    ]
    summary.control_flow = [cleanup_text(t) for t in summary.control_flow]
    summary.dependencies = [cleanup_text(t) for t in summary.dependencies]
    summary.how_it_fits = [cleanup_text(t) for t in summary.how_it_fits]


# -----------------------------------------------------------------------------
# Cache / DB
# -----------------------------------------------------------------------------


def compute_normalized_body_hash(impl: str) -> str:
    code = impl
    # Remove comments
    code = re.sub(r"//.*?$|/\*.*?\*/", "", code, flags=re.S | re.M)
    # Normalize whitespace
    code = re.sub(r"\s+", " ", code).strip()

    h = xxhash.xxh3_128()
    h.update(code)
    return h.hexdigest()


def compute_normalized_file_hash(content: str) -> str:
    code = content
    # Normalize whitespace only; keep comments for semantic context
    code = re.sub(r"\s+", " ", code).strip()
    h = xxhash.xxh3_128()
    h.update(code)
    return h.hexdigest()


def _build_bodyfile_func_map(
    funcs: Optional[List[FuncInfo]], repo_root: Path
) -> Dict[Path, List[FuncInfo]]:
    func_map: Dict[Path, List[FuncInfo]] = {}
    if not funcs:
        return func_map
    for func in funcs:
        bodyfile = func.bodyfile
        if not bodyfile:
            continue
        path = bodyfile
        if not path.is_absolute():
            path = (repo_root / path).resolve()
        else:
            path = path.resolve()
        func_map.setdefault(path, []).append(func)
    return func_map


def _render_xml_func_comment(func: FuncInfo) -> List[str]:
    brief = func.brief.strip() if func.brief else "TODO: clarify function purpose."
    lines = ["/*!"]
    lines.append(f"\t\\brief {brief}")
    if func.details:
        lines.append("")
        for ln in func.details.split("\n"):
            if ln.strip():
                lines.append(f"\t{ln}")
        lines.append("")
    for pname, _ptype in func.params:
        if not pname:
            continue
        pdesc = func.param_docs.get(pname)
        if pdesc:
            lines.append(f"\t\\param {pname} {pdesc}")
        else:
            lines.append(f"\t\\param {pname} TODO: clarify {pname} parameter.")
    if func.returns:
        if func.return_desc:
            lines.append(f"\t\\return {func.return_desc}")
        else:
            lines.append("\t\\return TODO: clarify return value.")
    lines.append("*/")
    return lines


def _build_original_comment_from_xml(func: FuncInfo) -> str:
    parts: List[str] = []
    if func.brief:
        parts.append(f"Brief: {func.brief}")
    if func.details:
        parts.append(f"Details: {func.details}")
    if func.params:
        param_lines: List[str] = []
        for pname, _ptype in func.params:
            if not pname:
                continue
            pdesc = func.param_docs.get(pname)
            if pdesc:
                param_lines.append(f"{pname}: {pdesc}")
        if param_lines:
            parts.append("Params: " + "; ".join(param_lines))
    if func.return_desc:
        parts.append(f"Returns: {func.return_desc}")
    return "\n".join(parts)


def _extract_comment_from_body(func: FuncInfo, repo_root: Path) -> str:
    bodyfile = func.bodyfile or func.file
    if not bodyfile.is_absolute():
        bodyfile = (repo_root / bodyfile).resolve()
    if not bodyfile.exists():
        return ""
    lines = read_file(bodyfile)
    start_idx = (func.bodystart or func.line) - 1
    if start_idx < 0 or start_idx >= len(lines):
        return ""
    func_id = get_func_identifier(func)
    decl_idx = find_declaration_line_for_body(lines, start_idx, func_id)
    if decl_idx == -1:
        return ""
    decl_idx = find_decl_anchor(lines, decl_idx)
    return extract_comments_for_declaration(lines, decl_idx)


def _build_original_class_comment_from_xml(class_info: ClassInfo) -> str:
    parts: List[str] = []
    if class_info.brief:
        parts.append(f"Brief: {class_info.brief}")
    if class_info.details:
        parts.append(f"Details: {class_info.details}")
    # if class_info.member_summaries:
    #     parts.append("Members: " + "; ".join(class_info.member_summaries))
    return "\n".join(parts)


def _augment_source_with_xml_doxygen(
    raw_lines: List[str],
    src_path: Path,
    func_map: Dict[Path, List[FuncInfo]],
) -> List[str]:
    lines = list(raw_lines)
    funcs = func_map.get(src_path.resolve())
    if not funcs:
        return lines

    insert_items: List[Tuple[int, List[str]]] = []
    for func in funcs:
        if not func.bodystart:
            continue
        func_id = get_func_identifier(func)
        body_idx = func.bodystart - 1
        decl_idx = find_declaration_line_for_body(lines, body_idx, func_id)
        if decl_idx == -1:
            continue
        decl_idx = find_decl_anchor(lines, decl_idx)
        if has_existing_doxygen(lines, decl_idx):
            continue

        existing_comment = extract_comment_block_above(lines, decl_idx)
        if existing_comment.strip():
            continue
        inline_comment = extract_inline_comment(lines[decl_idx])
        if inline_comment.strip():
            continue
        trailing_comment = extract_trailing_comment_lines(lines, decl_idx)
        if trailing_comment.strip():
            continue

        block_lines = _render_xml_func_comment(func)
        if decl_idx > 0 and lines[decl_idx - 1].strip() != "":
            block_lines = [""] + block_lines
        insert_items.append((decl_idx, block_lines))

    for decl_idx, block_lines in sorted(insert_items, key=lambda x: x[0], reverse=True):
        lines[decl_idx:decl_idx] = block_lines

    return lines


def make_func_key(func: FuncInfo, func_id: str) -> str:
    # As stable and unique as possible
    # return f"{func.file}:{func_id}:{func.definition}{func.argsstring}"
    return f"{func.definition}{func.argsstring}"


class FunctionCacheEntry(BaseModel):
    body_hash: str
    is_trivial: Optional[bool] = None


class CacheFile(BaseModel):
    version: int = 1
    functions: Dict[str, FunctionCacheEntry] = {}


def load_cache(path: Path) -> CacheFile:
    if not path.exists():
        return CacheFile()
    data = json.loads(path.read_text(encoding="utf-8"))
    return CacheFile(**data)


def save_cache(path: Path, cache: CacheFile) -> None:
    path.write_text(cache.model_dump_json(indent=2), encoding="utf-8")


# -----------------------------------------------------------------------------
# Core Logic
# -----------------------------------------------------------------------------
def generate_doxygen_comments(
    funcs: List[FuncInfo],
    repo_root: Path,
    llm: str,
    dry_run: bool,
    maximpl: int,
    thinking: Optional[object] = None,
    verbose: bool = False,
    cache_path: Optional[Path] = None,
    mcp_toolset: Optional[object] = None,
    mcp_enabled: bool = False,
    mcp_max_examples: int = 3,
    mcp_timeout: int = 20,
    details_mode: str = "mixed",
    update_trivial: bool = False,
    scope_dir: Optional[Path] = None,
    callsite_max: int = 3,
    callsite_context_lines: int = 30,
    callsite_roots: Optional[List[Path]] = None,
) -> int:
    insert_map: Dict[Path, List[Tuple[int, str]]] = {}
    impl_cleanup_map: Dict[Path, List[Tuple[int, str]]] = {}

    cache = load_cache(cache_path) if cache_path else CacheFile()

    scope_path = scope_dir.resolve() if scope_dir else None
    search_roots = callsite_roots or default_callsite_roots(repo_root)

    num_comments = 0
    ai_candidates = []
    thinking_label = "auto" if thinking is None else repr(thinking)
    print(f"[AI] thinking mode: {thinking_label}")

    # ---------------------------------------------------------------------------
    # 1. Build one canonical documentation target per function signature:
    # - Prefer declarations in header files
    # - If multiple header declarations exist, prefer the implementation location
    # - Otherwise fall back to first occurrence (typically implementation)
    # ---------------------------------------------------------------------------
    canonical_target_by_key: Dict[str, Tuple[Path, int, bool]] = {}
    funcs_by_key: Dict[str, List[FuncInfo]] = {}
    for f in funcs:
        key = make_func_key(f, get_func_identifier(f))
        funcs_by_key.setdefault(key, []).append(f)

    for key, fn_list in funcs_by_key.items():
        candidates: List[Tuple[Path, int, bool]] = []
        header_candidates: List[Tuple[Path, int, bool]] = []
        impl_candidate: Optional[Tuple[Path, int, bool]] = None

        for f in fn_list:
            p = f.file
            if not p.is_absolute():
                p = (repo_root / p).resolve()
            if not p.exists():
                continue
            if should_skip_project_file(p):
                continue

            is_header = p.suffix.lower() in {".h", ".hpp", ".hh", ".hxx"}
            cand = (p, f.line, is_header)
            candidates.append(cand)
            if is_header:
                header_candidates.append(cand)

            if f.bodyfile and f.bodystart:
                bp = f.bodyfile
                if not bp.is_absolute():
                    bp = (repo_root / bp).resolve()
                if bp.exists() and not should_skip_project_file(bp):
                    impl_candidate = (bp, int(f.bodystart), False)

        if len(header_candidates) > 1 and impl_candidate:
            canonical_target_by_key[key] = impl_candidate
            continue

        # Prefer header over non-header
        if header_candidates:
            chosen = header_candidates[0]
            for candidate in header_candidates[1:]:
                cand_path = os.path.normcase(str(candidate[0].resolve()))
                curr_path = os.path.normcase(str(chosen[0].resolve()))
                if cand_path < curr_path or (
                    cand_path == curr_path and candidate[1] < chosen[1]
                ):
                    chosen = candidate
            canonical_target_by_key[key] = chosen
            continue

        # No header; choose deterministic earliest among remaining candidates
        if candidates:
            chosen = candidates[0]
            for candidate in candidates[1:]:
                cand_path = os.path.normcase(str(candidate[0].resolve()))
                curr_path = os.path.normcase(str(chosen[0].resolve()))
                if cand_path < curr_path or (
                    cand_path == curr_path and candidate[1] < chosen[1]
                ):
                    chosen = candidate
            canonical_target_by_key[key] = chosen

    # ------------------------------------------------------
    # 2. Scan functions if they need Doxygen comments (fast)
    # ------------------------------------------------------
    for func in tqdm(funcs, desc="Scanning functions"):
        fpath = func.file
        if not fpath.is_absolute():
            fpath = (repo_root / fpath).resolve()
        if not fpath.exists():
            continue

        if scope_path and not is_under_dir(fpath, scope_path):
            continue
        if should_skip_project_file(fpath):
            continue

        # Ignore operators because they are usually too trivial
        # if func.name.startswith("operator"):
        #     continue

        if func.name == "va":
            continue

        if func.name in ("compile_time_assert", "assert_sizeof", "assert_offsetof"):
            continue

        if func.name.startswith("As<eventCallback_"):
            continue

        if func.is_pure_virtual:
            continue

        # HACK: only use files whose path contains 'Angles'
        # if (
        #     "Plane" != fpath.stem
        #     and "Str" != fpath.stem
        #     and "Vector" != fpath.stem
        #     and "BTree" != fpath.stem
        #     and "ParallelJobList" != fpath.stem
        #     and "Timer" != fpath.stem
        #     and "Parser" != fpath.stem
        #     and "Dict" != fpath.stem
        # ):
        #     continue

        # if func.name != "MakeNormalVectors":
        #     continue

        func_id = get_func_identifier(func)
        func_key = make_func_key(func, func_id)

        # Only process the canonical target for this signature
        canonical = canonical_target_by_key.get(func_key)
        if canonical is not None:
            fpath_norm = os.path.normcase(str(fpath.resolve()))
            canonical_path_norm = os.path.normcase(str(canonical[0].resolve()))
            if fpath_norm != canonical_path_norm or func.line != canonical[1]:
                continue

        impl = extract_implementation(func, repo_root, max_chars=maximpl)
        if not impl.strip():
            continue

        if is_trivial_destructor(func, impl):
            continue

        if impl.endswith(";"):
            if not _suppress_body_fetch_warning(func_id):
                print(
                    colored(
                        f" Could not fetch body for {func_id}\n",
                        "red",
                        "on_black",
                        ["bold", "blink"],
                    )
                )
            # impl = extract_implementation(func, repo_root, max_chars=maximpl)
            continue

        body_hash = compute_normalized_body_hash(impl)
        entry = cache.functions.get(func_key)

        # --- Read file to check Doxygen status ---
        lines = read_file(fpath)
        idx = max(0, min(func.line - 1, len(lines) - 1))
        decl_idx = find_decl_anchor(lines, idx)
        has_doxy = has_existing_doxygen(lines, decl_idx)

        computed_trivial = (
            is_trivial_getter(func, impl)
            or is_trivial_setter(func, impl)
            or is_trivial_forwarder(func, impl)
            or func.name.startswith("operator")
        )
        param_count = count_non_template_params(func)
        if details_mode in ("trivial", "min"):
            current_trivial = True
        elif details_mode == "mixed":
            current_trivial = param_count <= 3
        else:
            current_trivial = computed_trivial
        cached_trivial = entry.is_trivial if entry else None
        trivial_changed = False
        if update_trivial:
            trivial_changed = (
                cached_trivial is not None and cached_trivial != current_trivial
            )

        # --- Skip Criteria ---
        # 1) Body not changed and Doxygen already present -> skip completely
        if entry and entry.body_hash == body_hash and has_doxy and not trivial_changed:
            # optional: you could distinguish generated vs. manual comments here
            if entry.is_trivial is None:
                entry.is_trivial = current_trivial
            continue

        # 2) Doxygen exists, but no cache entry -> just update cache, no AI
        if has_doxy and not entry:
            cache.functions[func_key] = FunctionCacheEntry(
                body_hash=body_hash,
                is_trivial=current_trivial,
            )
            continue

        # At this point:
        # - either no Doxygen,
        # - or body changed -> AI can proceed
        bodypath = func.bodyfile
        if bodypath and not bodypath.is_absolute():
            bodypath = (repo_root / bodypath).resolve()

        ai_candidates.append(
            (func, func_id, func_key, impl, fpath, bodypath, body_hash, current_trivial)
        )

    # ----------------------------------
    # 3. Generate AI comments (slow)
    # ----------------------------------
    progressbar = tqdm(ai_candidates, desc="AI generating function comments")
    for (
        func,
        func_id,
        func_key,
        impl,
        fpath,
        bodypath,
        body_hash,
        trivial,
    ) in progressbar:
        progressbar.set_postfix(func=get_func_display_name(func))

        lines = read_file(fpath)
        idx = max(0, min(func.line - 1, len(lines) - 1))
        decl_idx = find_decl_anchor(lines, idx)
        orig_comment = _build_original_comment_from_xml(func)
        if not orig_comment.strip():
            orig_comment = extract_comments_for_declaration(lines, decl_idx)
        if not orig_comment.strip():
            orig_comment = _extract_comment_from_body(func, repo_root)
        if not orig_comment.strip():
            orig_comment = None

        if orig_comment:
            print(f"func {func.name} - original comment: {orig_comment}\n")

        mcp_context = None
        if mcp_cpp_server:
            mcp_context = mcp_collect_context(
                func,
                func_id,
                fpath,
                repo_root,
                mcp_cpp_server,
                max_examples=mcp_max_examples,
                timeout_s=mcp_timeout,
            )

        call_examples = find_callsite_examples(
            func,
            func_id,
            repo_root,
            search_roots,
            max_examples=callsite_max,
            context_lines=callsite_context_lines,
        )

        model, ok = ai_generate_comment(
            func,
            impl,
            llm,
            trivial,
            thinking=thinking,
            verbose=verbose,
            original_comment=orig_comment,
            mcp_context=mcp_context,
            call_examples=call_examples,
        )
        if (not ok) or model is None or not model.brief or model.brief == "MISSING":
            continue
        block = render_ai_block(model, trivial)

        insert_map.setdefault(fpath, []).append((func.line, block, func_id))
        num_comments += 1

        # Update Cache (although it gets not saved to disc yet)
        cache.functions[func_key] = FunctionCacheEntry(
            body_hash=body_hash,
            is_trivial=trivial,
        )

        if not bodypath or not bodypath.exists():
            continue

        if not func.bodystart:
            print("bodystart missing")
            continue

        # if fpath != bodypath:
        impl_cleanup_map.setdefault(bodypath, []).append((func.bodystart, func_id))

        # if num_comments > 5:
        #    break

    # ----------------------------------
    # 4. Insert comments
    # ----------------------------------
    total = 0
    for path, items in tqdm(insert_map.items(), desc="Inserting comments"):
        lines = read_file(path)
        items.sort(key=lambda x: x[0], reverse=True)

        changed = False
        for line_1b, block, func_id in items:
            func_idx = line_1b - 1  # Doxygen line of the function (0-based)
            if func_idx < 0 or func_idx >= len(lines):
                continue

            # 0) Remove inline comments on the declaration line
            lines[func_idx] = strip_inline_non_doxygen_comment(lines[func_idx])

            # 1) Find the actual declaration anchor (e.g., template<...> line)
            decl_idx = find_decl_anchor(lines, func_idx)

            # 2) Remove all existing Doxygen comments directly above the declaration
            #    (if you had multiple AI runs, they will be cleaned up one after another)
            while has_existing_doxygen(lines, decl_idx):
                decl_idx = remove_doxygen_block_above(lines, decl_idx)

            # 3) Clean up non-Doxygen comments directly above
            decl_idx = cleanup_comments_above(lines, decl_idx, func_id)

            # 4) Insert comment directly above the declaration anchor
            comment_start = decl_idx  # this is where the block will be inserted

            # 4) If there's no empty line directly above, insert an empty line
            if comment_start > 0 and lines[comment_start - 1].strip() != "":
                lines.insert(comment_start, "")
                comment_start += 1  # Comment moves down one line

            # 5) Insert comment block (without additional empty line after!)
            # Guard: ensure block comment is properly closed before insertion
            if block.lstrip().startswith(("/*!", "/**")):
                if "*/" not in block:
                    block = block.rstrip("\n") + "\n\t*/\n"
            block_lines = block.splitlines()
            # Second guard: check splitlines result as well
            if block_lines and block_lines[0].lstrip().startswith(("/*!", "/**")):
                if not any("*/" in ln for ln in block_lines):
                    block_lines.append("\t*/")
            lines[comment_start:comment_start] = block_lines

            changed = True
            total += 1

        if changed and not dry_run:
            write_file(path, lines)

    # ------------------------------------------------------------------------
    # 5. Clean up legacy autogenerated comments in implementation (.cpp) files
    # ------------------------------------------------------------------------
    for path, items in tqdm(impl_cleanup_map.items(), desc="Cleaning impl comments"):
        if not path.exists():
            continue

        lines = read_file(path)
        # Again, process from bottom to top to avoid index shifting issues
        items.sort(key=lambda x: x[0], reverse=True)

        changed = False
        for line_1b, func_id in items:
            # bodystart -> usually the first code line in the body
            body_idx = line_1b - 1
            if body_idx < 0 or body_idx >= len(lines):
                continue

            # 1) Walk up to the declaration line
            decl_idx = find_declaration_line_for_body(lines, body_idx, func_id)

            if decl_idx == -1:
                continue

            # 2) From there, move to the declaration anchor (e.g., template<...>)
            decl_idx = find_decl_anchor(lines, decl_idx)

            # 3) Remove banners / old comments above
            new_idx = cleanup_comments_above(
                lines, decl_idx, func_id, allow_skip_doxygen=True
            )
            if new_idx != decl_idx:
                changed = True

        if changed and not dry_run:
            write_file(path, lines)

    if cache_path:
        save_cache(cache_path, cache)

    return total


def generate_class_comments(
    classes: List[ClassInfo],
    project_root: Path,
    llm: str,
    thinking: Optional[object] = None,
    verbose: bool = False,
    dry_run: bool = False,
    scope_dir: Optional[Path] = None,
) -> int:
    if not classes:
        return 0

    scope_path = scope_dir.resolve() if scope_dir else None
    insert_map: Dict[Path, List[Tuple[int, List[str], str]]] = {}

    ai_candidates: List[ClassInfo] = []
    scanned = 0
    skipped_missing_file = 0
    skipped_struct = 0
    # skipped_non_header = 0
    skipped_out_of_scope = 0
    skipped_project_excluded = 0
    skipped_existing_doxygen = 0
    skipped_no_decl = 0

    for cls in tqdm(classes, desc="Scanning class comments"):
        scanned += 1
        if cls.kind == "struct":
            skipped_struct += 1
            continue
        cpath = cls.file
        if not cpath.is_absolute():
            cpath = (project_root / cpath).resolve()
        if not cpath.exists():
            skipped_missing_file += 1
            continue
        if scope_path and not is_under_dir(cpath, scope_path):
            skipped_out_of_scope += 1
            continue
        if should_skip_project_file(cpath):
            skipped_project_excluded += 1
            continue
        # if not is_header(cpath):
        #     skipped_non_header += 1
        #     continue

        lines = read_file(cpath)
        class_idx = max(0, min(cls.line - 1, len(lines) - 1))
        decl_idx = find_class_anchor(lines, class_idx)
        if has_existing_doxygen(lines, decl_idx):
            skipped_existing_doxygen += 1
            continue

        orig_comment = _build_original_class_comment_from_xml(cls)
        if not orig_comment.strip():
            orig_comment = extract_comments_for_declaration(lines, decl_idx)
        if not orig_comment.strip():
            orig_comment = None

        # if orig_comment:
        #     print(f"func {cls.name} - original comment: {orig_comment}\n")

        raw_decl = extract_class_declaration_block(lines, class_idx)
        if not raw_decl.strip():
            skipped_no_decl += 1
            continue

        class_info = ClassInfo(
            file=cls.file,
            line=cls.line,
            name=cls.name,
            kind=cls.kind,
            brief=cls.brief,
            details=cls.details,
            member_summaries=cls.member_summaries,
            raw_declaration=raw_decl,
            original_comment=orig_comment,
        )
        ai_candidates.append(class_info)

    skipped = scanned - len(ai_candidates)
    print(
        f"[class] scanned: {scanned}, candidates: {len(ai_candidates)}, skipped: {skipped}"
    )
    if skipped:
        print(
            "[class] skipped: "
            f"missing_file={skipped_missing_file}, "
            f"structs={skipped_struct}, "
            # f"non_header={skipped_non_header}, "
            f"out_of_scope={skipped_out_of_scope}, "
            f"project_excluded={skipped_project_excluded}, "
            f"existing_doxygen={skipped_existing_doxygen}, "
            f"no_decl={skipped_no_decl}"
        )

    ai_attempted = 0
    ai_generated = 0
    ai_failed = 0

    progressbar = tqdm(ai_candidates, desc="AI generating class comments")
    for class_info in progressbar:
        progressbar.set_postfix(cls=class_info.name)
        ai_attempted += 1
        model, ok = ai_generate_class_comment(
            class_info, llm, thinking=thinking, verbose=verbose
        )
        if not ok or model is None or not model.brief:
            ai_failed += 1
            continue
        ai_generated += 1

        block_lines = render_class_comment_block(class_info, model)
        cpath = class_info.file
        if not cpath.is_absolute():
            cpath = (project_root / cpath).resolve()
        insert_map.setdefault(cpath, []).append(
            (class_info.line, block_lines, class_info.name)
        )

    print(
        f"[class] AI attempted: {ai_attempted}, generated: {ai_generated}, failed: {ai_failed}"
    )

    total = 0
    for path, items in tqdm(insert_map.items(), desc="Inserting class comments"):
        lines = read_file(path)
        items.sort(key=lambda x: x[0], reverse=True)

        changed = False
        for line_1b, block_lines, class_name in items:
            class_idx = line_1b - 1
            if class_idx < 0 or class_idx >= len(lines):
                continue

            decl_idx = find_class_anchor(lines, class_idx)
            if has_existing_doxygen(lines, decl_idx):
                continue

            decl_idx = cleanup_comments_above(lines, decl_idx, class_name)

            if decl_idx > 0 and lines[decl_idx - 1].strip() != "":
                lines.insert(decl_idx, "")
                decl_idx += 1

            lines[decl_idx:decl_idx] = block_lines
            changed = True
            total += 1

        if changed and not dry_run:
            write_file(path, lines)

    return total


# -----------------------------------------------------------------------------
# Header Summary Generation
# -----------------------------------------------------------------------------
def generate_header_summaries(
    scope_dir: Path,
    project_root: Path,
    arch_root: Path,
    llm: str,
    thinking: Optional[object] = None,
    verbose: bool = False,
    funcs: Optional[List[FuncInfo]] = None,
    summarize_mode: Optional[str] = None,
) -> int:
    """
    Generates architecture-level Markdown summaries for all C++ header and
    source files under *scope_dir*. Each summary is written into a mirror
    tree under *arch_root* as ``<filename>.md`` and is also inserted as a
    file-level Doxygen block directly under the GPL header.

    A SHA-256 fingerprint of the source file is embedded in the first line so
    stale docs can be detected. Files whose hash has not changed are skipped.
    """
    if not scope_dir or not scope_dir.exists():
        print(f"[summary] scope-dir does not exist: {scope_dir}")
        return 0

    cpp_files = sorted(p for p in scope_dir.rglob("*") if is_header(p) or is_source(p))

    if not cpp_files:
        print(f"[summary] No C++ files found under {scope_dir}")
        return 0

    func_map = _build_bodyfile_func_map(funcs, project_root)
    summarize_mode = summarize_mode or "all"

    summary_agent = Agent(
        _build_ollama_model_from_llm(llm),
        output_type=HeaderSummaryModel,
        system_prompt=(
            "You are a senior C++ software architect. "
            "You are analyzing source code from Roguestrad, a RBDOOM-3-BFG engine fork which is a derivate of Doom 3 BFG by id Software.\n"
            "Summarize the provided C++ file into the requested structured sections.\n"
            "Be precise, technical, and concise. Focus on architectural significance.\n"
            "Use em-dash notation for key_types_and_functions entries, e.g.:\n"
            "  `idFoo` — short description of the class.\n"
            "  `idFoo::Bar(int x)` — what this method does.\n"
            "Every entry must start with a backticked identifier (type or function), then an em dash and description.\n"
            "Each bullet should be a complete, informative sentence or phrase.\n"
            "For control_flow, describe how data or logic flows at runtime.\n"
            "For dependencies, reference concrete header paths where possible.\n"
            "For how_it_fits, explain this component’s role in the larger engine.\n"
            "If details are unclear, write 'TODO: clarify ...' rather than guessing.\n"
        ),
    )

    if thinking is not None:
        summary_agent = Agent(
            _build_ollama_model_from_llm(llm),
            output_type=HeaderSummaryModel,
            system_prompt=summary_agent._system_prompts[0]
            if summary_agent._system_prompts
            else "",
            model_settings={"thinking": thinking},
        )

    generated = 0
    print(
        f"[summary] Generating summaries for {len(cpp_files)} files in {scope_dir} ..."
    )

    for src_file in tqdm(cpp_files, desc="Summarizing C++ files"):
        raw_lines = read_file(src_file)
        if is_source(src_file):
            augmented_lines = _augment_source_with_xml_doxygen(
                raw_lines, src_file, func_map
            )
        else:
            augmented_lines = raw_lines
        content_lines = _strip_file_doxygen_block(augmented_lines)
        content = "\n".join(content_lines)
        if not content.strip():
            continue

        rel_path = src_file.relative_to(project_root)
        file_sha = compute_normalized_file_hash(content)

        # Determine output path: mirror into arch_root
        out_path = arch_root / rel_path.with_suffix(rel_path.suffix + ".md")

        existing = _find_file_doxygen_block(raw_lines)

        if summarize_mode == "missing" and existing:
            if verbose:
                print(f"  [skip] {rel_path} (has \\file)")
            continue

        md_hash_matches = False
        if summarize_mode != "missing" and out_path.exists():
            first_line = out_path.read_text(encoding="utf-8", errors="ignore").split(
                "\n", 1
            )[0]
            if file_sha in first_line:
                md_hash_matches = True

        if (
            summarize_mode != "missing"
            and md_hash_matches
            and existing
            and existing[2] == file_sha
        ):
            if verbose:
                print(f"  [skip] {rel_path} (unchanged)")
            continue

        prompt = f"Analyze this C++ file: {rel_path.as_posix()}\n\nContent:\n{content}"

        try:
            result = summary_agent.run_sync(prompt)
            data = result.output
            _cleanup_summary_model(data)

            if summarize_mode != "markdown-only":
                _insert_file_doxygen_comment(src_file, rel_path, data, file_sha)

            md_lines = [
                f"<!-- doxygenix: xxh128={file_sha} -->",
                "",
                f"# {rel_path.as_posix()}",
                "",
                "## File Purpose",
            ]
            md_lines.extend(f"- {p}" for p in data.file_purpose)

            md_lines.append("")
            md_lines.append("## Core Responsibilities")
            md_lines.extend(f"- {r}" for r in data.core_responsibilities)

            md_lines.append("")
            md_lines.append("## Key Types and Functions")
            md_lines.extend(f"- {f}" for f in data.key_types_and_functions)

            md_lines.append("")
            md_lines.append("## Important Control Flow")
            md_lines.extend(f"- {c}" for c in data.control_flow)

            md_lines.append("")
            md_lines.append("## External Dependencies")
            md_lines.extend(f"- {d}" for d in data.dependencies)

            md_lines.append("")
            md_lines.append("## How It Fits")
            md_lines.extend(f"- {h}" for h in data.how_it_fits)
            md_lines.append("")  # trailing newline

            out_path.parent.mkdir(parents=True, exist_ok=True)
            out_path.write_text("\n".join(md_lines), encoding="utf-8")
            generated += 1

        except Exception as e:
            print(f"  [error] {rel_path}: {e}")

    print(f"[summary] Generated {generated} summaries.")
    return generated


# -----------------------------------------------------------------------------
# CLI
# -----------------------------------------------------------------------------
def run_doxygen(
    config_file: str,
    doxygen_exe: str = "doxygen",
    clean_xml: bool = True,
    repo_root: Optional[Path] = None,
) -> None:
    """
    Runs doxygen with a specified configuration file.
    Ensures that the XML output exists.
    Aborts with RuntimeError if doxygen reports an error.
    :param config_file: Path to the Doxyfile configuration (.cfg)
    :param doxygen_exe: Path to doxygen.exe (or simply "doxygen" if in PATH)
    :param clean_xml: Optional, deletes the XML folder before generation (recommended)
    """

    cfg = Path(config_file).resolve()
    if not cfg.exists():
        raise FileNotFoundError(f"Doxygen config file not found: {cfg}")

    # Target directory from the Doxyfile
    xml_output_dir = None
    with cfg.open("r", encoding="utf-8") as f:
        for line in f:
            if line.strip().startswith("OUTPUT_DIRECTORY"):
                xml_output_dir = line.split("=")[1].strip()
            if line.strip().startswith("XML_OUTPUT"):
                xml_subdir = line.split("=")[1].strip()

    # Defaults, if not specified
    xml_output_dir = Path(xml_output_dir or ".")
    xml_subdir = xml_subdir or "xml"

    xml_full_path = (xml_output_dir / xml_subdir).resolve()

    # Ensure OUTPUT_DIRECTORY exists before running Doxygen
    try:
        xml_output_dir.mkdir(parents=True, exist_ok=True)
    except Exception as exc:
        raise RuntimeError(
            f"Failed to create Doxygen output directory: {xml_output_dir}"
        ) from exc

    # Delete old XML folder to avoid stale files
    if clean_xml and xml_full_path.exists():
        print(f"[Doxygen] Removing previous XML directory: {xml_full_path}")
        shutil.rmtree(xml_full_path)

    print(f"[Doxygen] Starting Doxygen with config: {cfg}")
    print(f"[Doxygen] Output expected in: {xml_full_path}")

    try:
        if repo_root:
            import tempfile

            with tempfile.NamedTemporaryFile(
                "w", delete=False, suffix=".cfg", dir="."
            ) as tmp:
                tmp.write(f"@INCLUDE = {cfg.resolve().as_posix()}\n")
                tmp.write(f'INPUT = "{repo_root.resolve().as_posix()}"\n')
                tmp_cfg = tmp.name
            completed = subprocess.run(
                [doxygen_exe, tmp_cfg], capture_output=True, text=True, shell=False
            )
            Path(tmp_cfg).unlink()
        else:
            completed = subprocess.run(
                [doxygen_exe, str(cfg)], capture_output=True, text=True, shell=False
            )
    except Exception as exc:
        raise RuntimeError(f"Error starting Doxygen: {exc}") from exc

    # Log output
    if completed.stdout:
        print("[Doxygen STDOUT]")
        print(completed.stdout)
    if completed.stderr:
        print("[Doxygen STDERR]")
        print(completed.stderr)

    # Check for errors
    if completed.returncode != 0:
        raise RuntimeError(f"Doxygen failed (Exit {completed.returncode}).")

    # Verify that XML was actually created
    if not xml_full_path.exists() or not any(xml_full_path.iterdir()):
        raise RuntimeError(f"Doxygen did not create XML data under {xml_full_path}")

    print("[Doxygen] Successfully generated.\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--xml-dir", default="docs/doxygen/xml")
    ap.add_argument("--project-root", default=".")
    ap.add_argument("--scope-dir", default="idlib/bv")
    ap.add_argument("--llm", default="ollama/qwen3-coder")
    # ap.add_argument("--llm", default="ollama/gemma4")
    ap.add_argument("--max-impl", type=int, default=6000)
    ap.add_argument("--apply", action="store_true")

    ap.add_argument("--cache-file", default="doxygenix_func_cache.json")
    ap.add_argument("--callsite-max", type=int, default=3)
    ap.add_argument("--callsite-context", type=int, default=30)
    ap.add_argument(
        "--thinking",
        default="auto",
        choices=["auto", "true", "false", "low", "medium", "high"],
    )
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument(
        "--details",
        default="mixed",
        choices=["trivial", "min", "mixed", "max"],
        help="Detail mode: trivial/min (force trivial), mixed (default, trivial except >3 params), max (full heuristics).",
    )
    ap.add_argument(
        "--skip-class-comments",
        action="store_true",
        help="Skip class comment generation.",
    )
    ap.add_argument(
        "--update-trivial",
        action="store_true",
        help="Update comments when trivial/non-trivial status changes",
    )
    ap.add_argument(
        "--summarize-files",
        nargs="?",
        const="all",
        choices=["all", "missing", "markdown-only"],
        help="Generate summaries: 'all' (default), 'missing' (only files without \\file, skip hash), or 'markdown-only' (no \\file insertion)",
    )
    ap.add_argument(
        "--summarize-only",
        action="store_true",
        help="Skip function comment generation and only build summaries",
    )

    ap.add_argument(
        "--arch-root",
        default="docs/architecture",
        help="Root directory for architecture .md output (default: architecture)",
    )
    args = ap.parse_args()

    xml_dir = Path(args.xml_dir)
    project_root = Path(args.project_root)
    scope_dir = Path(args.scope_dir) if args.scope_dir else None
    cache_base = project_root
    # cache_base = project_root / ".cache" / "doxygenix"
    # cache_base.mkdir(parents=True, exist_ok=True)
    cache_path = cache_base / args.cache_file

    thinking_setting: Optional[object] = None
    if args.thinking and args.thinking != "auto":
        lowered = args.thinking.lower()
        if lowered in {"true", "false"}:
            thinking_setting = lowered == "true"
        else:
            thinking_setting = lowered

    details_mode = args.details
    if details_mode == "trivial":
        details_mode = "min"

    print(f"[AI] comment mode: {details_mode}")
    print("Create Doxygen XML …")
    run_doxygen(
        "Doxyfile-xmlgen.cfg", doxygen_exe="doxygen.exe", repo_root=project_root
    )

    funcs, classes = parse_doxygen_xml(
        xml_dir, project_root, prefer_decl=True, collect_classes=True
    )
    total = 0
    if not args.summarize_only:
        total = generate_doxygen_comments(
            funcs,
            project_root,
            llm=args.llm,
            thinking=thinking_setting,
            verbose=args.verbose,
            dry_run=False,  # not args.apply,
            maximpl=args.max_impl,
            cache_path=cache_path,
            update_trivial=args.update_trivial,
            scope_dir=scope_dir,
            callsite_max=args.callsite_max,
            callsite_context_lines=args.callsite_context,
            details_mode=details_mode,
        )

    print(f"Comments inserted: {total}")

    if not args.summarize_only and not args.skip_class_comments:
        if total > 0:
            print("Recreate Doxygen XML …")
            run_doxygen(
                "Doxyfile-xmlgen.cfg", doxygen_exe="doxygen.exe", repo_root=project_root
            )
        funcs, classes = parse_doxygen_xml(
            xml_dir, project_root, prefer_decl=True, collect_classes=True
        )
        class_total = generate_class_comments(
            classes,
            project_root=project_root,
            llm=args.llm,
            thinking=thinking_setting,
            verbose=args.verbose,
            dry_run=False,
            scope_dir=scope_dir,
        )
        print(f"Class comments inserted: {class_total}")

    # -- Header summary generation -------------------------------------------
    if args.summarize_files:
        arch_root = Path(args.arch_root)
        generate_header_summaries(
            scope_dir=scope_dir,
            project_root=project_root,
            arch_root=arch_root,
            llm=args.llm,
            thinking=thinking_setting,
            verbose=args.verbose,
            funcs=funcs,
            summarize_mode=args.summarize_files,
        )


if __name__ == "__main__":
    main()
