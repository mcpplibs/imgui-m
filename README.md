# imgui-m

Dear ImGui **1.92.8** 的 C++23 模块封装，保留上游 API，提供可选窗口与后端入口。

## 快速开始

使用 mcpp **2026.10.24.1**。以下为候选版本，索引发布后可直接创建项目。

```sh
mcpp new ui --template ocornut.imgui@1.92.8.1
cd ui
mcpp run
```

默认 `headless` 模板生成并检查绘制顶点，不需要桌面或 GPU。
桌面项目使用 `:window` 或 `:docking` 模板；它们支持 `--smoke-test`，绘制三帧后退出。

## 已有项目

```toml
[dependencies]
"ocornut.imgui" = { version = "1.92.8.1", features = ["app"] }
```

```cpp
import imgui.core;
import imgui.app;
int main() {
    return ImGui::App::run([] {
        ImGui::Begin("Hello");
        ImGui::TextUnformatted("Hello from mcpp");
        ImGui::End();
    });
}
```

## 能力边界

目标平台为 Linux、macOS、Windows；桌面模板需要可用的窗口系统和 OpenGL 运行环境。
`app` 启用窗口循环，`docking` 启用停靠，`viewports` 启用独立窗口；`docking-full` 组合后两者。
`ImGui::App::Options::max_frames` 为 0 时交互运行，非零时限制绘制帧数。

## 文档

[用法、模块与后端](docs/usage.md) · [无窗口示例](examples/basic) ·
[桌面示例](examples/app_minimal) · [MIT](LICENSE)
