# AGENTS

This file provides instructions for AI agents to understand the layout of the Roguestrad repository, run builds/tests, and follow project conventions.
Roguestrad is an RBDOOM-3-BFG fork for indie standalone development.

## Repository Structure

- `/bin/`                  : Built binaries by configuration (Debug/Release/RelWithDebInfo)
- `/build/`                : Build outputs and intermediates
- `/cmake/`                : CMake scripts and configuration
- `/engine/`               : Engine source code
  - `/engine/aas/`         : Area Awareness system
  - `/engine/collision/`   : Collision system
  - `/engine/decls/`       : Declarations and definition system
  - `/engine/framework/`   : Core framework and runtime
  - `/engine/imgui/`       : ImGui integration
  - `/engine/renderer/`    : Rendering system
  - `/engine/shaders/`     : Engine shader sources
  - `/engine/sound/`       : Audio system (OpenAL/XAudio2/stub)
  - `/engine/stub/`        : Stubbed subsystems
  - `/engine/swf/`         : SWF/Flash UI system
  - `/engine/sys/`         : Platform/system layers (common/posix/sdl/win32)
  - `/engine/ui/`          : UI system
  - `/engine/vr/`          : VR integration
- `/extern/`               : External dependencies (submodules/vendor drops)
  - `/extern/imgui-node-editor/`
  - `/extern/nvrhi/`
  - `/extern/openvr/`
  - `/extern/ShaderMake/`
- `/game/`                 : Game code
  - `/game/ai/`            : AI logic
  - `/game/anim/`          : Animation system
  - `/game/bots/`          : Gladiator Bot logic from Quake 3
  - `/game/entities/`      : Entity implementations
  - `/game/gamesys/`       : Game systems
  - `/game/menus/`         : Menu definitions
  - `/game/monsters/`      : Monster implementations
  - `/game/physics/`       : Gameplay physics
  - `/game/player/`        : Player code
  - `/game/script/`        : Script system
  - `/game/weapons/`       : Weapons
- `/idlib/`                : Core shared libraries/utilities
- `/libs/`                 : Third-party libraries (e.g., ffmpeg, freetype, lua, openal-soft, zlib)
- `/tools/`                : Tooling and utilities
  - `/tools/compilers/`    : Content compilers
    - `/tools/compilers/aas/`    : `runAAS` BSPC replacement to generate AAS files
    - `/tools/compilers/rogmap/` : Standalone BSP compiler with lighting support (like q3map)
  - `/tools/imgui/`        : ImGui-based tools (afeditor, lighteditor, util)
  - `/tools/typeinfo/`     : Typeinfo tool to generate RTTI files for game code

## Conventions

- Keep engine code in `/engine/` and game modules in `/game/`.
- Place third-party dependencies under `/libs/` or `/extern/` (submodules/vendor drops).
- Build artifacts must stay under `/build/` or `/bin/` and should not be checked in if avoidable.


## Notes for Agents

- Avoid editing build outputs under `/build/`.
- Prefer edits in `/engine/`, `/game/`, `/idlib/`, or `/tools/` depending on the task.
- Check formatting config in the repository root (`.clang-format-*`, `astyle-*.ini`).
