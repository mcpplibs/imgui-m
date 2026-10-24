# imgui

> Dear ImGui through C++23 modules, with optional app and docking interfaces.

**English** · [简体中文](README.zh.md) · [Upstream](https://github.com/ocornut/imgui) · [Releases](https://github.com/mcpplibs/imgui-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![mcpp-community](https://img.shields.io/badge/mcpp--community-mwrap-blue)](https://github.com/mcpp-community)
[![CI](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml)

Keep `ImGui::` APIs. Core, backend contracts and the optional app facade remain separate.

## Modules

- `import mcpplibs.mwrap.imgui;`: Core API and macro mappings
- `import mcpplibs.mwrap.imgui.core;`: Dear ImGui contexts, frames and widgets
- `import mcpplibs.mwrap.imgui.app;`: Optional app frame loop; requires app
- `import mcpplibs.mwrap.imgui.backend;`: Backend contracts and shared types
- `import mcpplibs.mwrap.imgui.backend.headless;`: Display-independent backend
- `import mcpplibs.mwrap.imgui.backend.glfw_opengl3;`: GLFW/OpenGL3 window backend

> More Modules API -> [Usage and API](docs/usage.md)

## Getting started

```sh
mcpp new ui --template mcpplibs.mwrap.imgui@1.92.8
cd ui
mcpp run
```

```toml
[dependencies]
mcpplibs.mwrap.imgui = "1.92.8"
```

```cpp
import mcpplibs.mwrap.imgui;
int main() { ImGui::CreateContext(); ImGui::DestroyContext(); }
```

## Features

| Feature | Purpose |
|---|---|
| `app` | App frame loop |
| `docking` | Dockable panels |
| `viewports` | Separate OS windows |
| `docking-full` | docking + viewports |

The provider is upstream `1.92.8-docking`; desktop templates need a display.

## Examples

Default: display-independent context/frame checks. `:window` and `:docking` require a desktop.
[Examples](examples) · [Usage](docs/usage.md) · [Architecture](docs/architecture.md)

## Build and runtime

Linux, macOS and Windows CI. OpenGL/GLX runtime adapters do not redistribute the host GPU driver.

## License

[Apache-2.0](LICENSE); the upstream provider has its own MIT notice.

Wrapper: Apache-2.0, mcpp-community. [Original upstream README and licenses](upstream/imgui/). Upstream licenses remain unchanged.
