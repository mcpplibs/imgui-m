# imgui

> Dear ImGui 的 C++23 模块封装，提供可选 app 与 docking 接口。

[English](README.md) · **简体中文**

[Releases](https://github.com/mcpplibs/imgui-m/releases) · [mcpp](https://github.com/mcpp-community/mcpp) · [Index](https://github.com/mcpplibs/mcpp-index)

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![CI](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml)

使用 mcpp **2026.10.24.1**。新 mwrap 身份在索引发布后可直接安装。

## 核心特性

保留 `ImGui::` API，core、后端契约与可选 app facade 分层。

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

## 模块一览

根模块导出 `.core` 与 `.macros`；`.app` 按需启用。
`.backend`、`.backend.headless`、`.backend.glfw_opengl3` 保留显式后端选择。

## 特性

`app` 开启帧循环；`docking`、`viewports` 按需叠加，`docking-full` 组合两者。
供应者为上游 1.92.8-docking，单独记录 docking 来源。

## 示例

默认模板验证无显示依赖的 context/frame；`:window` 与 `:docking` 需要桌面。
[示例](examples) · [使用](docs/usage.md) · [架构](docs/architecture.md)

## 工具链与运行时

Linux、macOS、Windows CI 覆盖；OpenGL/GLX 运行时适配不分发宿主 GPU 驱动。

## 许可证

[MIT](LICENSE)；上游供应者保留其 MIT 通知。
