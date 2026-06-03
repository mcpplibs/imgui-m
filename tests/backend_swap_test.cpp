#include <gtest/gtest.h>

import imgui.core;
import imgui.backend;
import imgui.backend.headless;
import imgui.backend.glfw_opengl3;

namespace B = ImGui::Backend;

// Two independent implementations of the same compile-time contract.
static_assert(B::BackendApi<B::Headless>);
static_assert(B::BackendApi<B::GlfwOpenGL3>);

// The SAME application loop, parameterized only by the backend type —
// swapping backends is one import + one alias; the loop is untouched (I3).
template <class Backend>
int run_frames(int frames) {
    if (!Backend::InitPlatform()) return -1;
    auto* window = Backend::CreateWindow(320, 240, "swap-test");
    if (window == nullptr) { Backend::TerminatePlatform(); return -2; }
    Backend::MakeContextCurrent(window);

    ImGuiContext* ctx = ImGui::CreateContext();
    ImGui::SetCurrentContext(ctx);
    if (!Backend::Init(window)) {
        ImGui::DestroyContext(ctx);
        Backend::DestroyWindow(window);
        Backend::TerminatePlatform();
        return -3;
    }

    int rendered = 0;
    for (int i = 0; i < frames && !Backend::WindowShouldClose(window); ++i) {
        Backend::PollEvents();
        Backend::NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("swap");
        ImGui::TextUnformatted("same loop, different backend");
        ImGui::End();
        ImGui::Render();

        const auto fb = Backend::FramebufferSize(window);
        Backend::Viewport(0, 0, fb.width, fb.height);
        Backend::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        Backend::ClearColorBuffer();
        Backend::RenderDrawData(ImGui::GetDrawData());
        Backend::SwapBuffers(window);
        if (ImGui::GetDrawData() != nullptr) ++rendered;
    }

    Backend::Shutdown();
    ImGui::DestroyContext(ctx);
    Backend::DestroyWindow(window);
    Backend::TerminatePlatform();
    return rendered;
}

// Compile-time proof the identical loop builds against the windowed backend
// too (not executed here — CI is headless).
template int run_frames<B::GlfwOpenGL3>(int);

TEST(BackendSwapTest, HeadlessRunsTheSameLoop) {
    EXPECT_EQ(run_frames<B::Headless>(3), 3);
}

TEST(BackendSwapTest, HeadlessWindowLifecycle) {
    auto* w = B::Headless::CreateWindow(64, 32, "x");
    ASSERT_NE(w, nullptr);
    EXPECT_FALSE(B::Headless::WindowShouldClose(w));
    B::Headless::SetWindowShouldClose(w, true);
    EXPECT_TRUE(B::Headless::WindowShouldClose(w));
    const auto fb = B::Headless::FramebufferSize(w);
    EXPECT_EQ(fb.width, 64);
    EXPECT_EQ(fb.height, 32);
    B::Headless::DestroyWindow(w);
}
