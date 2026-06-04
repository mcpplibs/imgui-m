# imgui

> Dear ImGui 的 C++23 模块化封装 — `import` 即用 · 跨平台 · 特性可插拔

[![Release](https://img.shields.io/github/v/release/mcpplibs/imgui-m)](https://github.com/mcpplibs/imgui-m/releases)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![Module](https://img.shields.io/badge/module-ok-green.svg)](https://en.cppreference.com/w/cpp/language/modules)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

| [mcpp 构建工具](https://github.com/mcpp-community/mcpp) · [包索引 mcpp-index](https://github.com/mcpp-community/mcpp-index) · [Dear ImGui 上游](https://github.com/ocornut/imgui) · [Issues](https://github.com/mcpplibs/imgui-m/issues) · [Releases](https://github.com/mcpplibs/imgui-m/releases) |
|:---:|
| [![CI](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/mcpplibs/imgui-m/actions/workflows/ci.yml) |

## 核心特性

- **纯模块导入** — `import imgui.core;`,消费者代码零 `#include`
- **三档使用体验** — `imgui.app` 一行出窗口(默认档)/ `imgui.backend` 编译期契约(自动档)/ `imgui.backend.<impl>` 显式选后端(专家档),换后端 = 改一行 import + alias
- **特性可插拔** — `features = ["docking-full"]` 解锁面板停靠 + 拖出主窗口变独立 OS 窗口
- **不绑定工具链、不打包 GL 驱动** — Linux / macOS / Windows 三平台 CI;GL 运行时由 mcpp 生态自动闭合

## 快速开始

```bash
mcpp new myapp --template gui && cd myapp && mcpp run   # 窗口直接出现
```

或在已有项目中手动接入:

```toml
[dependencies]
imgui = "0.0.5"
```

```cpp
import imgui.core;
import imgui.app;

int main() {
    return ImGui::App::run([] {
        ImGui::Begin("hello");
        ImGui::TextUnformatted("mcpp + imgui");
        ImGui::End();
    });
}
```

## 模块一览

| 模块 | 说明 |
|---|---|
| `imgui.core` | Dear ImGui 核心:context / frame / widgets / draw-data |
| `imgui.app` | Tier-0 facade:平台默认后端 + 一行帧循环 |
| `imgui.backend` | 后端契约:`BackendApi` concept + 共享类型(`GlConfig`/`Error`/`FbSize`) |
| `imgui.backend.glfw_opengl3` | GLFW + OpenGL3 窗口后端(单一 `Backend` 类型) |
| `imgui.backend.headless` | 无显示后端:同一业务循环跑 CI / 逻辑测试 |

后端实现模块**不 re-export** `imgui.core`,消费者需各自显式 `import`;换后端只改 import 行与一行 `using Backend = ...;`。

## Features

| Feature | 说明 |
|---|---|
| `docking` | 主窗口内面板停靠(Wayland 安全) |
| `viewports` | 面板拖出主窗口分离为独立 OS 窗口(X11 / Windows / macOS) |
| `docking-full` | 组合糖 = `["docking", "viewports"]`,完整 docking 体验 |

```toml
[dependencies]
imgui = { version = "0.0.5", features = ["docking-full"] }
```

docking/viewports 源码来自上游 docking tag(`compat.imgui 1.92.8-docking`,主线超集——特性关闭时行为与主线一致)。需 mcpp ≥ 0.0.47。

## 示例

| 示例 | 内容 |
|---|---|
| [`examples/app_minimal`](examples/app_minimal) | facade 三行出窗口 |
| [`examples/docking`](examples/docking) | IDE 式四分屏 + 面板分离为独立 OS 窗口 |
| [`examples/minimal_window`](examples/minimal_window) | 显式后端 API 的最小窗口(带诊断) |
| [`examples/glfw_opengl3`](examples/glfw_opengl3) | 显式后端的完整事件循环 |
| [`examples/basic`](examples/basic) | headless 核心 API(CI 可跑) |

```bash
cd examples/docking && mcpp run
```

## 工具链与运行时

包不固定工具链(mcpp 解析环境默认,如 Linux glibc gcc);GL/GLX 运行时由生态包 `compat.glx-runtime` 透传闭合,不打包驱动。解析结果可用 `mcpp why` / `mcpp self doctor` 查看。

> [!NOTE]
> 早期版本,接口可能调整。问题与想法欢迎提 [issue](https://github.com/mcpplibs/imgui-m/issues)。

## License

封装代码 MIT;Dear ImGui 本体 MIT(经 `compat.imgui` 提供)。
