# imgui

> Dear ImGui through C++23 modules, with optional app and docking interfaces.

**English** · [简体中文](README.zh.md)

[Releases](https://github.com/mcpplibs/imgui-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![CI](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml)

Use mcpp **2026.10.24.1**. The new mwrap identity becomes installable after index publication.

## Characteristics

Keep `ImGui::` APIs. Core, backend contracts and the optional app facade remain separate.

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

## Modules

The root exports `.core` and `.macros`; `.app` is opt-in.
`.backend`, `.backend.headless` and `.backend.glfw_opengl3` keep explicit backend selection.

## Features

`app` enables the run loop; `docking` and `viewports` are additive; `docking-full` combines both.
The provider is upstream 1.92.8-docking, a separately recorded docking snapshot.

## Examples

Default: display-independent context/frame checks. `:window` and `:docking` require a desktop.
[Examples](examples) · [Usage](docs/usage.md) · [Architecture](docs/architecture.md)

## Build and runtime

Linux, macOS and Windows CI. OpenGL/GLX runtime adapters do not redistribute the host GPU driver.

## License

[MIT](LICENSE); the upstream provider has its own MIT notice.
