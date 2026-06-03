# imgui

C++23 module package for Dear ImGui on mcpp.

This package uses `compat.imgui` for upstream Dear ImGui sources and provides
module interfaces for the core API plus GLFW/OpenGL3 backend entry points.

## Modules

- `imgui.core`
  - Core Dear ImGui context, frame, widget, and draw-data APIs.
- `imgui.backend`
  - Generic backend abstraction layer: shared value types (`GlConfig`, `Error`,
    `FbSize`), `RecommendedGlConfig()` cross-platform defaults, and the
    compile-time `BackendApi` contract every backend satisfies. No platform code.
- `imgui.backend.platform.glfw`
  - GLFW platform piece: window/event management + `ImGui_ImplGlfw_*` binding.
- `imgui.backend.renderer.opengl3`
  - OpenGL3 renderer piece: GL framebuffer ops + `ImGui_ImplOpenGL3_*` binding.
- `imgui.backend.glfw_opengl3`
  - Concrete backend: assembles the GLFW platform and OpenGL3 renderer pieces
    into a single `ImGui::Backend::GlfwOpenGL3` type satisfying `BackendApi`.
    Re-exports `imgui.backend` (shared types) but **not** `imgui.core`.

Backends expose one `Backend` type with a uniform static API, so swapping
backend is just a different `import` plus a one-line `using Backend = ...;`
alias — the rest of the consumer code is identical. Cross-platform GL/GLSL
configuration (incl. macOS forward-compat) is handled by `RecommendedGlConfig()`,
which `CreateWindow`/`Init` use by default.

## Dependencies

The root package depends on:

- `compat.imgui = "1.92.8"`
- `compat.glfw = "3.4"`
- `compat.opengl = "2026.05.31"`

The repository does not vendor Dear ImGui sources. Source and header files come
from compat packages through mcpp dependency metadata.

## Quick Start

```bash
mcpp build
mcpp test
cd examples/basic
mcpp run
```

Expected headless example output:

```text
Dear ImGui 1.92.8 module frame ok
```

## Backend Example

The minimal GLFW/OpenGL3 window example uses the combined backend module and
does not use `#include` in consumer code:

```bash
cd examples/minimal_window
mcpp build
mcpp run
```

It requires an OpenGL runtime that is visible to the mcpp/xlings runtime. If
GLFW cannot create the window, the example prints the GLFW error text. The
`compat.opengl` package supplies OpenGL registry/header content; it does not
bundle a platform `libGL`/GLX runtime.

The larger GLFW/OpenGL3 example builds on CI but should be run only in an
environment with a working display and OpenGL context:

```bash
cd examples/glfw_opengl3
mcpp build
mcpp run
```

## Consumer Usage

```toml
[dependencies]
imgui = "0.0.1"
```

Then import the modules you need:

```cpp
import imgui.core;
import imgui.backend.glfw_opengl3;
using Backend = ImGui::Backend::GlfwOpenGL3;   // swap import + alias to switch backends

int main() {
    Backend::InitGlfw();
    auto* window = Backend::CreateWindow(960, 540, "demo");   // cross-platform GL hints
    Backend::MakeContextCurrent(window);

    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);
    Backend::Init(window);                                    // RecommendedGlConfig() default
    // ... frame loop: Backend::NewFrame / ImGui::* / Backend::RenderDrawData ...
    Backend::Shutdown();
    ImGui::DestroyContext(context);
    Backend::DestroyWindow(window);
    Backend::TerminateGlfw();
}
```

## Module Surface

`src/core.cppm` wraps `imgui.h` through a global module fragment and exports a
tested core API surface:

- Types: `ImGuiContext`, `ImFontAtlas`, `ImGuiIO`, `ImDrawData`, `ImVec2`,
  `ImVec4`.
- Functions: context lifecycle, `GetIO`, `NewFrame`, `Begin`, `Button`,
  `TextUnformatted`, `End`, `Render`, and `GetDrawData`.

`imgui.core` exports a curated core subset; for rarely used APIs not yet
exported, a consumer translation unit can still `#include <imgui.h>` directly
alongside the module imports.

Backend modules adapt the official Dear ImGui backend headers internally and
expose a single `Backend` type per backend (uniform static API constrained by
`ImGui::Backend::BackendApi`). Consumer code only needs module imports for the
surfaces exposed by this package.

## Verification

The `0.0.1` release state is verified with mcpp `0.0.44`:

- `mcpp build`
- `mcpp test`
- `cd examples/basic && mcpp run`
- `cd examples/minimal_window && mcpp build`
- `cd examples/glfw_opengl3 && mcpp build`

## License

The wrapper code in this repository is MIT licensed. Dear ImGui is MIT licensed
and provided by the `compat.imgui` package.
