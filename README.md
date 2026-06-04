# imgui

C++23 module package for Dear ImGui on mcpp.

This package uses `compat.imgui` for upstream Dear ImGui sources and provides
module interfaces for the core API plus GLFW/OpenGL3 backend entry points.

## Modules

Three tiers of consumer UX:

- **Tier-0** `imgui.app` — the convenience facade. Owns the whole window/context
  lifecycle; you supply only a per-frame UI callback.
- **Tier-1** automatic via the `imgui.backend` contract — write backend-agnostic
  code against the `BackendApi` concept and shared types.
- **Tier-2** explicit `imgui.backend.<impl>` — import a concrete backend module
  and alias it (`using Backend = ...;`); swap backend with one import + alias.

- `imgui.app`
  - Tier-0 facade. `ImGui::App::run(opts, ui)` / `ImGui::App::run(ui)` drive the
    full GLFW/OpenGL3 lifecycle using the default backend. `export import`s
    `imgui.core` so a consumer gets the `ImGui::*` surface for free.
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

## Features

- `docking` — exports the Dock* API surface (`DockSpace`, `DockSpaceOverViewport`,
  `SetNextWindowDockID`, `IsWindowDocked`, ...) and makes the `imgui.app` facade
  enable `ImGuiConfigFlags_DockingEnable` automatically:

  ```toml
  [dependencies]
  imgui = { version = "0.0.4", features = ["docking"] }
  ```

  See `examples/docking/`. Sources come from upstream's docking tag
  (`compat.imgui 1.92.8-docking`, a superset of mainline — identical behavior
  while the feature is off).

## Dependencies

The root package depends on:

- `compat.imgui = "1.92.8"`
- `compat.glfw = "3.4"`
- `compat.opengl = "2026.05.31"`

The repository does not vendor Dear ImGui sources. Source and header files come
from compat packages through mcpp dependency metadata.

### Toolchain and GL runtime

The package does not pin a toolchain; mcpp resolves the environment/default
toolchain. The GL runtime is closed by mcpp/mcpp-index (`compat.glx-runtime`,
pulled in transitively by `compat.glfw` on Linux), not bundled by this package.

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

Then import the modules you need.

Tier-0 (`imgui.app` facade — least code):

```cpp
import imgui.core;
import imgui.app;

int main() {
    return ImGui::App::run([] {
        ImGui::Begin("hi");
        ImGui::TextUnformatted("imgui.app facade");
        ImGui::End();
    });
}
```

Tier-2 (explicit backend module + alias — full control):

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
- `cd examples/app_minimal && mcpp build`
- `cd examples/minimal_window && mcpp build`
- `cd examples/glfw_opengl3 && mcpp build`

## License

The wrapper code in this repository is MIT licensed. Dear ImGui is MIT licensed
and provided by the `compat.imgui` package.
