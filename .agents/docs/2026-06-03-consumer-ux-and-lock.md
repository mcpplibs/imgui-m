# imgui-m: Consumer UX, Unpinned Toolchain, and Lock Policy

> 创建: 2026-06-03 · Owner: sunrisepeak · 仓库: mcpplibs/imgui-m
> 关联 Master Plan: `/home/speak/workspace/github/agentdocs/2026-06-03-mcpp-ecosystem-architecture-plan.md`
> (本文是该计划工作流 **W3 (imgui-m)** 的子设计文档,见 Master Plan §3.3 / §6)

本文记录 imgui-m 侧的三件事:不锁定工具链(unpinned)的决定与理由、三档递进式
消费者 UX(默认 `imgui.app` / 自动契约 / 显式后端模块)、以及 lockfile 策略
(GL 运行时闭合需要 `compat.glx-runtime`)。所有决定都对齐 Master Plan 的
「Capability → Resolution Plan,两平面 + 三档收敛」统一模型,不为单次验收做特殊设计。

---

## 1. 不锁定工具链(unpinned)

### 决定

本包**不在任何 `mcpp.toml` 中声明 `[toolchain]`**(根包与所有 examples 均不 pin)。
mcpp 解析环境/默认工具链;GL 运行时由 mcpp/mcpp-index 闭合
(`compat.glx-runtime`,Linux 上由 `compat.glfw` 传递依赖引入),**不由本包打包**。

### 理由

- **关注点分离 / 两平面**(Master Plan §2.1–2.2)。工具链与 GL 主机运行时属于
  *环境/主机平面*,不是包的内容。包在 `mcpp.toml` 里 pin 工具链会把环境决定
  硬编码进可移植的包元数据,破坏「同一包在不同环境用合适工具链」的能力。
- **三档模型的"默认档"**:零配置可达,意图而非机制。包不写工具链 = 让 mcpp 用
  环境默认;需要时由消费者/CI 在*环境层*显式叠加(见 §4),而不是在包里固化。
- **可移植**:macOS / Windows runner 不需要也不应继承 Linux 的工具链选择;不 pin
  让每个平台各自解析本地最优工具链。

---

## 2. 三档递进式消费者 UX

对齐 Master Plan §2.3「默认(零配置) → 自动匹配(意图驱动) → 显式指定(局部叠加)」。

### Tier-0 — 默认 / `imgui.app` facade(最少代码)

`imgui.app` 拥有完整的窗口/上下文生命周期,消费者只提供每帧 UI 回调。
模块接口里以 inline header-style 模板提供 `run`,UI 回调为模板参数,**无 `<functional>`
依赖**。`export import imgui.core;`,因此消费者免费拿到 `ImGui::*` 表面。

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

API:

```cpp
export namespace ImGui::App {
    struct Options { int width = 1280; int height = 720; const char* title = "mcpp imgui app"; };
    template <class Ui> int run(Options opts, Ui&& ui); // 完整生命周期
    template <class Ui> int run(Ui&& ui);               // 用默认 Options
}
```

`run()` 流程:InitGlfw → CreateWindow(RecommendedGlConfig) → MakeContextCurrent →
SwapInterval(1) → CreateContext/SetCurrentContext → Backend::Init → 循环
(PollEvents / Backend::NewFrame / ImGui::NewFrame / `ui()` / ImGui::Render /
FramebufferSize+Viewport+ClearColor(0.10,0.10,0.12,1)+ClearColorBuffer+
RenderDrawData(GetDrawData)+SwapBuffers)→ 退出时 Shutdown / DestroyContext /
DestroyWindow / TerminateGlfw。成功返回 0,初始化失败返回非 0(并打印
`Backend::LastError`)。

### Tier-1 — 自动 / `imgui.backend` 契约(意图驱动)

针对 `BackendApi` 概念与共享类型写**后端无关**代码;具体后端是哪个由上层选择。
任何满足契约的后端都能代入,消费者代码不变。

```cpp
import imgui.core;
import imgui.backend;   // BackendApi 概念 + GlConfig/Error/FbSize + RecommendedGlConfig()

template <ImGui::Backend::BackendApi Backend>
void frame(typename Backend::Window* w) {
    Backend::PollEvents();
    Backend::NewFrame();
    ImGui::NewFrame();
    /* ... UI ... */
    ImGui::Render();
    const auto fb = Backend::FramebufferSize(w);
    Backend::Viewport(0, 0, fb.width, fb.height);
    Backend::RenderDrawData(ImGui::GetDrawData());
    Backend::SwapBuffers(w);
}
```

### Tier-2 — 显式 / `imgui.backend.<impl>` 模块 + alias(完全控制)

显式导入一个具体后端模块并 alias;换后端 = 换 import + 改一行 alias,其余不变。

```cpp
import imgui.core;
import imgui.backend.glfw_opengl3;
using Backend = ImGui::Backend::GlfwOpenGL3;   // 换后端只需改这两行

int main() {
    Backend::InitGlfw();
    auto* window = Backend::CreateWindow(960, 540, "demo");
    Backend::MakeContextCurrent(window);
    ImGuiContext* ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
    Backend::Init(window);
    while (!Backend::WindowShouldClose(window)) { /* 手写帧循环 */ }
    Backend::Shutdown();
    ImGui::DestroyContext(ctx);
    Backend::DestroyWindow(window);
    Backend::TerminateGlfw();
}
```

复杂度单调下沉:Tier-0 最少代码 → Tier-2 最大控制。三档共享同一套
`RecommendedGlConfig()` 跨平台 GL/GLSL 默认与同一后端契约。

---

## 3. Lockfile 策略

### 策略

`mcpp.lock` 必须反映当前 index 的解析结果。GL 运行时闭合依赖 `compat.glx-runtime`
(Linux 上 `compat.glfw` 的传递依赖,把 host libGL/GLX 软链进 install 目录,
供 rpath/RUNPATH 透传)。lock 应随当前 index 重生成,不手工编辑。

重生成方式:仓库根目录 `mcpp update`(清空 lock)→ `mcpp build`(重解析并重写 lock)。

### 实测现状(诚实记录)

- `compat.glx-runtime@2026.06.03` 确已存在于 index
  (`compat.glfw.lua` linux `deps` 含 `compat.glx-runtime`),并在 BMI 解析的依赖树中可见。
- 但用 **mcpp 0.0.46** 重生成后,根 `mcpp.lock` 仅记录**直接依赖**
  (`compat.glfw` / `compat.imgui` / `compat.opengl`),**未**展开 `compat.glx-runtime`
  等传递依赖。即:当前 lock 格式只锁直接依赖,GL 运行时闭合发生在解析/链接期,
  而非在 lock 文本里展开。
- 另注:本仓 `.gitignore` 忽略 `mcpp.lock`,故 lock 不入库。

这对应 Master Plan §1 的 **R3**(lock 相对 index 的 glx-runtime 接线 stale)与
W1 的链接闭合修复(R2 rpath)。**完整的传递闭合(让 glx-runtime 显式落到 lock /
RUNPATH)需要 mcpp 核心侧的解析/rpath 工作就绪**;imgui-m 侧已做到「不 pin + 用当前
index 重生成 lock + 文档化策略」。待 mcpp 核心改动落地后,重生成的 lock/链接将
自然纳入 glx-runtime,无需改本包。

---

## 4. CI 过渡步骤(显式环境默认,非特殊设计)

Linux CI 今天会红,根因是 Master Plan **R1**:全新机器 mcpp bootstrap 默认装
musl-static 工具链,无法链接 glibc 世界的 X11/GL 栈。

修复:在 `.github/workflows/ci.yml` 的 "Install project tools" 之后、"Build library"
之前,加**一步**(仅 Linux):

```yaml
- name: Set default toolchain (env)
  if: runner.os == 'Linux'
  shell: bash
  run: mcpp toolchain default gcc@16.1.0
```

为什么这不是"特殊设计":这是在 *CI 环境层* 显式声明环境默认工具链
(三档模型里的"显式档,局部叠加"),与最终架构一致;**包仍不 pin**。它是过渡而非长期:
待 Master Plan W1(mcpp bootstrap 默认改为 glibc 工具链)合并后,此步可直接删除。
macOS / Windows 已通过,保持不变;矩阵与 `continue-on-error` 设置不变。

---

## 5. 与 Master Plan 的映射

| 本文小节 | Master Plan 对应 |
|---|---|
| §1 不 pin 工具链 | §2.1 两平面 / §2.3 三档默认档 / §1 R1 |
| §2 三档 UX | §2.3 三档递进;W3 的 `imgui.app` (Tier-0) |
| §3 lock 策略 | §1 R3(lock stale)/ W1 运行闭合(R2 rpath)/ §6 W3 |
| §4 CI 过渡步骤 | §7 注 100–102(过渡:CI 环境默认,W1 后删) |

本文是 Master Plan 列出的三份子文档之一(imgui-m 侧);另两份分别在 mcpp 核心
(runtime-closure-and-toolchain-defaults)与 mcpp-index(capability-runtime-metadata)。
