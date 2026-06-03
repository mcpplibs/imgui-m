# Architecture

This repository adapts Dear ImGui to the mcpp module workflow while keeping
upstream source ownership in compat packages.

## Goals

- Let consumers use explicit module imports:
  - `import imgui.core;`
  - `import imgui.backend;` (generic abstraction layer: shared types + contract)
  - `import imgui.backend.platform.glfw;`
  - `import imgui.backend.renderer.opengl3;`
  - `import imgui.backend.glfw_opengl3;`
- Provide a uniform, compile-time-constrained backend surface so swapping a
  backend is just a different import plus a one-line alias, with cross-platform
  GL/GLSL defaults handled by the abstraction layer.
- Keep the package source small: module wrappers and backend implementation
  translation units only.
- Use `compat.imgui`, `compat.glfw`, and `compat.opengl` as the source and
  header providers.
- Provide both a headless core example and a real GLFW/OpenGL3 window example.

## Layout

```text
.
|-- mcpp.toml
|-- src/
|   |-- core.cppm
|   `-- backends/
|       |-- backend.cppm            (abstraction layer: types + BackendApi)
|       |-- platform_glfw.cppm      (GLFW platform piece)
|       |-- renderer_opengl3.cppm   (OpenGL3 renderer piece)
|       |-- glfw_opengl3.cppm       (concrete Backend assembly)
|       |-- glfw_impl.cpp
|       `-- opengl3_impl.cpp
|-- tests/
|   |-- imgui_test.cpp
|   `-- backend_test.cpp
`-- examples/
    |-- basic/
    |-- minimal_window/
    `-- glfw_opengl3/
```

## Package Configuration

The root `mcpp.toml` declares package `imgui`, lists the module wrappers and
backend implementation translation units, and depends on compat packages:

```toml
[build]
sources = [
  "src/core.cppm",
  "src/backends/backend.cppm",
  "src/backends/platform_glfw.cppm",
  "src/backends/renderer_opengl3.cppm",
  "src/backends/glfw_opengl3.cppm",
  "src/backends/glfw_impl.cpp",
  "src/backends/opengl3_impl.cpp",
]

[dependencies]
compat.imgui = "1.92.8"
compat.glfw = "3.4"
compat.opengl = "2026.05.31"
```

The wrapper package does not vendor `third_party/imgui`. Upstream headers,
core sources, and backend files are read from `compat.imgui`.

## Module Wrappers

`src/core.cppm` adapts upstream `imgui.h` internally, then exports selected
types and functions with explicit `using` declarations. This keeps consumer code
on `import imgui.core;` instead of requiring direct header usage.

Backend module interfaces similarly adapt the official backend headers
internally and export explicit wrapper functions. The implementation `.cpp`
files compile upstream backend sources from `compat.imgui/backends` so consumers
do not need to copy backend sources into their own package.

## Validation

Primary proof points:

```bash
mcpp build
mcpp test
cd examples/basic && mcpp run
cd ../minimal_window && mcpp build
cd ../glfw_opengl3 && mcpp build
```

The minimal and full GLFW/OpenGL3 examples are real windowed examples. They are
build-checked in headless CI and intended to be run on a machine with a working
display/OpenGL context.
