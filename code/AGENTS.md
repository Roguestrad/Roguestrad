# AGENTS

This file provides instructions for AI agents to understand the layout of the Roguestrad repository, run builds/tests, and follow project conventions.
Roguestrad is a RBDOOM-3-BFG fork for indie standalone development.

## Repository Structure

- `/bin/`                  : Built binaries by configuration (Debug/Release/RelWithDebInfo)
- `/build/`                : Build outputs and intermediates
- `/cmake/`                : CMake helper scripts
- `/engine/`               : Engine source code
  - `/engine/aas/`         : Area Awareness system
  - `/engine/collision/`   : Collision system
  - `/engine/decls/`       : Declarations and definition system
  - `/engine/framework/`   : Core framework and runtime
  - `/engine/imgui/`       : ImGui integration
  - `/engine/renderer/`    : Rendering system
    - `/engine/renderer/backend/`          : Executes render cmdlist with either D3D12 or Vulkan
      - `/engine/renderer/backend/passes/` : Special render passes like TAA
    - `/engine/renderer/frontend/`         : idRenderSystem delegates most calls to frontend which handles resource loading, occlusion culling and builds a cmdlist for the renderer backend of what is to draw
      - `/engine/renderer/frontend/fonts/`  : .dat and .ttf loading
      - `/engine/renderer/frontend/images/` : Image loading and caching to .bimage files
        - `/engine/renderer/frontend/images/baked/`    : Header dumps of preprocessed image files
        - `/engine/renderer/frontend/images/color/`    : Color conversion tools
        - `/engine/renderer/frontend/images/encoding/` : BC1-6 encoding / decoding
      - `/engine/renderer/frontend/models/` : Model support for ase, lwo, obj, .md3, .md5, .gltf
    - `/engine/renderer/NVRHI/`            : NVRHI specific implementations of mostly the renderer backend
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
  - `/game/bots/`          : Gladiator Bot logic ported from Quake 3
  - `/game/entities/`      : Entity implementations
  - `/game/gamesys/`       : Game systems
  - `/game/menus/`         : C++ logic for Flash menus/hud instead of ActionScript
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
  - `/tools/typeinfo/`     : Typeinfo tool called by cmake to generate RTTI files from the game code (game/gamesys/GameTypeInfo.{h/cpp})

## Conventions

- Keep engine code in `/engine/` and game specific code in `/game/`.
- Place third-party dependencies under `/libs/` or `/extern/` (submodules/vendor drops).
- Build artifacts must stay under `/build/` or `/bin/` and should not be checked in if avoidable.

## Documentation
Write Doxygen comments:

- **Trivial mode**: emit only a single line Doxygen comment:
  `//! <short, clear sentence>`
- **Full mode**: emit a block comment using `/*! ... */` with `\brief` as the first line.
- **No redundancy**: do not restate the signature in prose.
- **No speculation**: if details are unclear, write `TODO: clarify ...` instead of guessing.
- **Params/return**: include only when meaningful and justified by the implementation.
- **No Markdown fences** inside comments.
- **No implementation-side docs** when a header declaration is already documented.
- Indent comments with tabs.

### Doxygen Examples
For general layout what the current .h or .cpp file implements
```
/*! \file CmdSystem.h
  \brief Console command execution and command text buffering.
  Any number of commands can be added in a frame from several different
  sources. Most commands come from either key bindings or console line input,
  but entire text files can be execed.

  Command execution takes a null terminated string, breaks it into tokens,
  then searches for a command or variable that matches the first token.
*/
```

For class descriptions:
```
/*! \class idBounds
  \brief Axis Aligned Bounding Box
  A more detailed class description.
*/
```

For complex functions:
```
/*!
	\brief Computes the intersection line between two planes.

	\param plane The other plane.
	\param start Output start point of the intersection line.
	\param dir Output direction of the intersection line.
	\return true if the planes intersect in a line, false if they are parallel.
*/
bool PlaneIntersection( const idPlane& plane, idVec3& start, idVec3& dir ) const;
```

For simple functions like trivial getters and small functions of a few lines:
```
//! Returns the dimension of the plane.
int GetDimension() const;
```

## Notes for Agents

- Avoid editing build outputs under `/build/`.
- Prefer edits in `/engine/`, `/game/`, `/idlib/`, or `/tools/` depending on the task.
- Don't make any edits under `/extern/` or in any system library headers.
- Don't run cmake yourself and only use the clang linter feedback in the IDE to correct your code.
