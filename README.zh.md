# imgui

> Dear ImGui 的 C++23 模块封装，提供可选 app 与 docking 接口。

[English](README.md) · **简体中文** · [Upstream](https://github.com/ocornut/imgui) · [Releases](https://github.com/mcpplibs/imgui-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![CI](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml)

保留 `ImGui::` API，core、后端契约与可选 app facade 分层。

## 模块

- `import mcpplibs.mwrap.imgui;`: 核心 API 与宏映射
- `import mcpplibs.mwrap.imgui.core;`: Dear ImGui 上下文、帧与控件
- `import mcpplibs.mwrap.imgui.app;`: 可选应用帧循环，需 app feature
- `import mcpplibs.mwrap.imgui.backend;`: 后端契约与共享类型
- `import mcpplibs.mwrap.imgui.backend.headless;`: 无显示依赖后端
- `import mcpplibs.mwrap.imgui.backend.glfw_opengl3;`: GLFW/OpenGL3 窗口后端

> More Modules API -> [模块与 API](docs/usage.md)

## 快速开始

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

## 特性

| Feature | 用途 |
|---|---|
| `app` | 应用帧循环 |
| `docking` | 面板停靠 |
| `viewports` | 独立系统窗口 |
| `docking-full` | 组合 docking 与 viewports |

供应者为上游 `1.92.8-docking`；桌面模板需要显示环境。

## 示例

默认模板验证无显示依赖的 context/frame；`:window` 与 `:docking` 需要桌面。
[示例](examples) · [使用](docs/usage.md) · [架构](docs/architecture.md)

## 工具链与运行时

Linux、macOS、Windows CI 覆盖；OpenGL/GLX 运行时适配不分发宿主 GPU 驱动。

## 许可证

[MIT](LICENSE)；上游供应者保留其 MIT 通知。
