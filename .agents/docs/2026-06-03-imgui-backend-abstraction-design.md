# imgui-m: Backend Abstraction & Cross-Platform Design

> 状态: design (approved) → implementing
> 分支: `codex/fix-imgui-window-clear` (PR #3)
> Last updated: 2026-06-03
> 目标: 把 imgui-m 拆成 `imgui.core` + 通用 backend 抽象层 + 可替换的后端实现,
> 让"换后端只改 import + 一行 alias",并把工程从 Linux-only 推进到 Linux/macOS/Windows。

## 1. 背景与问题

当前结构(`0.0.1`)有三个限制:

1. **后端职责越界 + 组合模块大量手工转发。** `imgui.backend.glfw_opengl3`
   把整个子模块 API 逐函数手抄转发一遍(~每个函数一处),N 处同步漂移风险。
2. **没有显式的"后端抽象层"。** 不同后端之间没有一个被编译期约束的统一表面,
   消费者无法做到"换后端 = 换 import"。
3. **工程只验证过 Linux/x86_64。** `mcpp.toml` 只声明 `linux` toolchain,CI 只有
   `ubuntu-latest`;示例硬编码 `ConfigureOpenGL(3,3,true,false)`,在 macOS 上会因
   缺少 forward-compat + core profile 直接创建上下文失败。

## 2. 设计目标

- `imgui.core` 与 backend 彻底分层;backend 实现模块**不 re-export `imgui.core`**。
- 提供**通用 backend 抽象层(契约)**:用 C++20 `concept` 在编译期约束每个后端的
  统一 API 形状。
- 每个后端对外暴露**单一 `Backend` 类型**(全 static 方法),职责包含:窗口生命周期、
  GL/帧缓冲操作、ImGui platform/renderer 绑定、错误诊断。
- **用法一致**:`import imgui.core; import imgui.backend.<impl>;` 后,业务代码在不同
  后端间逐字复用,换后端只改 import 行 + 一行 `using Backend = ...`。
- 跨平台:Linux / macOS / Windows 都能 `mcpp build`;`RecommendedGlConfig()` 自动给出
  各平台正确的 GL/GLSL 配置。

### 非目标 (Non-Goals)

- 不打包 `libGL`/GLX/EGL/Mesa/驱动(沿用既有 GL runtime boundary 约定)。
- 本次不实现 SDL/Vulkan/Metal/DX 后端,只**预留**扩展位。
- 不追求把 Dear ImGui 全量 API 手抄进 `imgui.core`(见 §7)。
- 不在 headless CI 里依赖真实显示;真实窗口运行是独立的、需显示环境的步骤。

## 3. 模块分层

```
imgui.core                          # 纯 Dear ImGui:context / frame / widgets / draw-data
imgui.backend                       # ★契约层:共享值类型 + BackendApi concept(无实现、不依赖平台库)
│
├─ imgui.backend.platform.glfw      # 平台片:窗口/事件 + ImGui_ImplGlfw_*(可复用拼装单元)
├─ imgui.backend.renderer.opengl3   # 渲染片:GL 操作 + ImGui_ImplOpenGL3_*(可复用拼装单元)
│
└─ imgui.backend.glfw_opengl3       # ★组合实现:装配 platform+renderer 为满足契约的单一 Backend 类型
   (预留) imgui.backend.sdl3_opengl3, imgui.backend.glfw_vulkan, ...
```

设计理由:

- **契约层 (`imgui.backend`)** 是"通用 backend 抽象层"的载体:只放跨后端共享的*值类型*
  与*概念*,不放任何实现,不 `import` 平台库。它给出统一表面的"形状定义"。
- **platform / renderer 片**对应 Dear ImGui 自身的 platform/renderer 拆分,是可复用的
  拼装单元,避免 `GLFW×OpenGL3`、`GLFW×Vulkan`、`SDL×OpenGL3` 的组合爆炸。
- **组合实现**把一个 platform 片和一个 renderer 片装配成对外的单一 `Backend` 类型,
  并用 `static_assert(BackendApi<...>)` 在编译期保证它符合契约。

### import 规则(硬约束)

- 所有 backend 模块**只 `import imgui.core`(私有,仅为签名用到 `ImDrawData` 等)**,
  **绝不 `export import imgui.core`**。消费者必须自己 `import imgui.core`。
- 但组合实现模块**应 `export import imgui.backend`**:`GlConfig`/`Error`/`FbSize`
  这些共享值类型是后端公开表面的一部分,消费者需要直接命名它们(例如接住
  `Backend::LastError()` 的返回值)。"不 re-export" 的约束只针对 `imgui.core`,
  不针对抽象层本身。
- 消费者两种合法用法:
  - 纯逻辑 / headless:`import imgui.core;`
  - 带窗口:`import imgui.core; import imgui.backend.<impl>;`(两个 import 都要显式写)

## 4. 契约层 `imgui.backend`

只含跨后端共享的值类型、跨平台默认配置、以及统一 API 的 `concept`。

```cpp
export module imgui.backend;

import imgui.core;   // 私有:仅 RenderDrawData(ImDrawData*) 等签名需要,不 re-export

export namespace ImGui::Backend {
    // ---- 共享值类型 ----
    struct GlConfig {
        int  major         = 3;
        int  minor         = 3;
        bool coreProfile   = true;
        bool forwardCompat = false;
        const char* glsl   = nullptr;   // nullptr => 让 renderer 选默认
    };
    struct Error  { int code = 0; const char* description = nullptr; };
    struct FbSize { int width = 0; int height = 0; };
    struct Rgba   { float r = 0, g = 0, b = 0, a = 1; };

    // ---- 跨平台正确默认值 ----
    // macOS: 强制 GL>=3.2 core + forwardCompat,glsl="#version 150"
    // Linux/Windows: GL 3.3 core,glsl="#version 130"/nullptr
    GlConfig RecommendedGlConfig();

    // ---- 编译期契约:每个后端必须满足的统一 API 形状 ----
    template <class T>
    concept BackendApi = requires (
        typename T::Window* w, const char** desc, ImDrawData* dd,
        int i, float f, GlConfig cfg
    ) {
        typename T::Window;
        { T::InitGlfw() }              -> std::same_as<bool>;
        { T::TerminateGlfw() }        -> std::same_as<void>;
        { T::CreateWindow(i, i, "") } -> std::same_as<typename T::Window*>;
        { T::DestroyWindow(w) }       -> std::same_as<void>;
        { T::MakeContextCurrent(w) }  -> std::same_as<void>;
        { T::Init(w, cfg) }           -> std::same_as<bool>;
        { T::NewFrame() }             -> std::same_as<void>;
        { T::Viewport(i, i, i, i) }   -> std::same_as<void>;
        { T::ClearColor(f, f, f, f) } -> std::same_as<void>;
        { T::ClearColorBuffer() }     -> std::same_as<void>;
        { T::RenderDrawData(dd) }     -> std::same_as<void>;
        { T::SwapBuffers(w) }         -> std::same_as<void>;
        { T::PollEvents() }           -> std::same_as<void>;
        { T::WindowShouldClose(w) }   -> std::same_as<bool>;
        { T::FramebufferSize(w) }     -> std::same_as<FbSize>;
        { T::Shutdown() }             -> std::same_as<void>;
        { T::LastError() }            -> std::same_as<Error>;
    };
}
```

> 说明:`concept` 约束的是"类型 T 是否具备这组 static 成员",因此每个后端用一个
> `struct` 承载 API。这既给出编译期保证,又让所有后端的调用写法完全一致。

## 5. 后端实现:单一 `Backend` 类型

### 5.1 平台片 `imgui.backend.platform.glfw`

封装 GLFW 窗口/事件 + `ImGui_ImplGlfw_*`,导出一个 `struct GlfwPlatform`(全 static)。
内部使用 `#define GLFW_INCLUDE_NONE` + `<GLFW/glfw3.h>` + `<imgui_impl_glfw.h>`。
负责:`InitGlfw/Terminate/CreateWindow/.../FramebufferSize/LastError` 与 ImGui 平台帧。
GL 上下文创建按传入 `GlConfig` 设置 window hints(含 macOS forward-compat)。

### 5.2 渲染片 `imgui.backend.renderer.opengl3`

封装 `<imgui_impl_opengl3.h>` + loader,导出 `struct OpenGL3Renderer`(全 static):
`Init(glsl)/Shutdown/NewFrame/Viewport/ClearColor/ClearColorBuffer/RenderDrawData`。

### 5.3 组合实现 `imgui.backend.glfw_opengl3`

```cpp
export module imgui.backend.glfw_opengl3;

import imgui.core;                       // 私有,不 re-export
export import imgui.backend;             // 契约层类型(GlConfig/Error/FbSize)随后端公开
import imgui.backend.platform.glfw;      // 私有装配单元
import imgui.backend.renderer.opengl3;   // 私有装配单元

export namespace ImGui::Backend {
    struct GlfwOpenGL3 {
        using Window  = GlfwPlatform::Window;
        using Monitor = GlfwPlatform::Monitor;

        static bool    InitGlfw()                 { return GlfwPlatform::InitGlfw(); }
        static void    TerminateGlfw()            { GlfwPlatform::TerminateGlfw(); }
        static Window* CreateWindow(int w, int h, const char* t,
                                    Monitor* m = nullptr, Window* s = nullptr);
        static bool    Init(Window* w, GlConfig cfg = RecommendedGlConfig());
        static void    NewFrame();                // renderer + platform 帧
        static void    Viewport(int, int, int, int);
        static void    ClearColor(float, float, float, float);
        static void    ClearColorBuffer();
        static void    RenderDrawData(ImDrawData*);
        static void    SwapBuffers(Window*);
        static void    PollEvents();
        static bool    WindowShouldClose(Window*);
        static FbSize  FramebufferSize(Window*);
        static void    Shutdown();                // renderer + platform 关闭
        static Error   LastError();
        // ... 其余诊断/回调按需
    };

    static_assert(BackendApi<GlfwOpenGL3>);   // 编译期保证用法一致
}
```

要点:
- **取消逐函数手工转发的"组合模块"形态** —— 现在是单一类型、单处定义,内部委托
  platform/renderer 片。
- `Init` 内部:`GlfwPlatform::InitForOpenGL(...)` 失败即返回;再 `OpenGL3Renderer::Init(cfg.glsl)`,
  失败则回滚 platform 关闭(保留既有错误回滚语义)。
- **不隐式清屏**:`RenderDrawData` 只画 ImGui,清屏交给应用(`ClearColor`/`ClearColorBuffer`),
  以支持"先渲染自己的场景、ImGui 作 overlay"——这正是 PR #3 的设计取向。

## 6. 用法一致(消费者视角)

```cpp
import imgui.core;
import imgui.backend.glfw_opengl3;
using Backend = ImGui::Backend::GlfwOpenGL3;   // ← 换后端只改这两行(import + alias)

int main() {
    if (!Backend::InitGlfw()) { /* Backend::LastError() */ return 1; }
    auto* window = Backend::CreateWindow(960, 540, "demo");
    Backend::MakeContextCurrent(window);

    auto* ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
    Backend::Init(window);                       // 默认 RecommendedGlConfig(),跨平台正确

    while (!Backend::WindowShouldClose(window)) {
        Backend::PollEvents();
        Backend::NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("hello"); ImGui::TextUnformatted("..."); ImGui::End();
        ImGui::Render();

        auto fb = Backend::FramebufferSize(window);
        Backend::Viewport(0, 0, fb.width, fb.height);
        Backend::ClearColor(0.08f, 0.09f, 0.10f, 1.0f);
        Backend::ClearColorBuffer();
        Backend::RenderDrawData(ImGui::GetDrawData());
        Backend::SwapBuffers(window);
    }
    Backend::Shutdown();
    ImGui::DestroyContext(ctx);
    Backend::DestroyWindow(window);
    Backend::TerminateGlfw();
}
```

换成将来的 `imgui.backend.sdl3_opengl3` 时,业务循环逐字不变;`concept` +
`static_assert` 保证两个后端表面完全一致。

## 7. `imgui.core` 导出策略(可扩展性)

当前 `core.cppm` 是手维护的 `using` 白名单,每加一个 ImGui 函数都要补一行,不可持续。
本设计确立约定:

- `imgui.core` 导出**常用核心子集**(context 生命周期、frame、常用 widgets、draw-data、
  基础类型 `ImVec2/ImVec4/ImGuiContext/ImGuiIO/ImFontAtlas/ImDrawData`)。
- 对暂未导出的冷门 API,提供**显式逃生舱口**:文档约定消费者可在自己的 TU 里
  `#include <imgui.h>`(global module fragment)直接调用,与模块用法并存。
- 不在本次扩成全量;后续按需增量补充导出子集。

> 本次实现范围:在 §3 分层与 §4–§6 落地的前提下,`imgui.core` 至少补齐 `ImVec4` 与
> 示例所需 widgets;全量导出策略留作后续。

## 8. 跨平台工程化

### 8.1 toolchain(不锁定 / unpinned)

本包**不在任何 `mcpp.toml` 中锁定 toolchain**(没有 `[toolchain]` 段)。
mcpp 解析环境/默认 toolchain;GL 运行时由 mcpp/mcpp-index 闭合
(`compat.glx-runtime`,Linux 上由 `compat.glfw` 传递依赖引入),不由本包打包。

> 过渡说明:目前全新机器上 mcpp bootstrap 默认是 musl-static 工具链,无法链接
> 宿主 X11/GL 运行时。CI 在 Linux 上用一个显式环境默认步骤
> (`mcpp toolchain default gcc@16.1.0`)绕开,而不是在 `mcpp.toml` 里锁定。
> 待 mcpp bootstrap 默认改为 glibc 工具链后即可移除该步骤。详见
> `.agents/docs/2026-06-03-consumer-ux-and-lock.md`。

### 8.2 GL/GLSL 配置

`RecommendedGlConfig()` 按平台返回:

| 平台 | major.minor | coreProfile | forwardCompat | glsl |
|---|---|---|---|---|
| Linux   | 3.3 | true | false | `#version 130` |
| Windows | 3.3 | true | false | `#version 130` |
| macOS   | 3.2 | true | **true** | `#version 150` |

示例改用 `Backend::Init(window)`(默认即 `RecommendedGlConfig()`),**移除硬编码**
`ConfigureOpenGL(3,3,true,false)`。

### 8.3 CI 矩阵

`.github/workflows/ci.yml` 由单 `ubuntu-latest` 扩为 `ubuntu-latest` /
`macos-latest` / `windows-latest` 的 build-check 矩阵:

- 每个 runner:`mcpp build` + `mcpp test` + build 三个 examples。
- headless 例子 `examples/basic` 可在所有平台 `mcpp run`。
- **真实窗口运行**保持为需显示环境的条件步骤(不进 headless CI 必经路径)。

## 9. 影响面 / 文件清单

新增:
- `src/backends/backend.cppm` (契约层)
- `src/backends/platform_glfw.cppm` (+ 既有 `glfw_impl.cpp`)
- `src/backends/renderer_opengl3.cppm` (+ 既有 `opengl3_impl.cpp`)

改造:
- `src/backends/glfw_opengl3.cppm` → 单一 `GlfwOpenGL3` 类型
- `src/core.cppm` → 补 `ImVec4` 等(§7)
- `tests/backend_test.cpp` → 针对新表面 + `static_assert(BackendApi<...>)`
- `examples/minimal_window`、`examples/glfw_opengl3` → 统一 `Backend::` 用法 + `RecommendedGlConfig()`
- `mcpp.toml`(sources;**不含** toolchain)、`.github/workflows/ci.yml`(矩阵 + Linux 环境默认 toolchain 过渡步骤)
- `docs/architecture.md`、`README.md` 同步

> 旧的 `imgui.backend.glfw` / `imgui.backend.opengl3` 自由函数命名空间被
> platform/renderer 片取代。如需保留兼容名,可在过渡期保留薄别名;本次按"干净切换"处理。

## 10. 验证

- `mcpp build` / `mcpp test`(三平台 CI)
- `cd examples/basic && mcpp run`(headless,跨平台)
- `cd examples/minimal_window && mcpp build`
- `cd examples/glfw_opengl3 && mcpp build`
- **真实窗口运行**(本机 DISPLAY + OpenGL runtime 可用时):
  `cd examples/minimal_window && mcpp run` 观察窗口出现、拖动无残影。
