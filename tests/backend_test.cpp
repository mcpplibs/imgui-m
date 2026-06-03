#include <gtest/gtest.h>

import imgui.core;
import imgui.backend;
import imgui.backend.platform.glfw;
import imgui.backend.renderer.opengl3;
import imgui.backend.glfw_opengl3;

namespace Backend = ImGui::Backend;

// The contract is satisfied at compile time. This also documents that any new
// backend must pass the same assertion.
static_assert(Backend::BackendApi<Backend::GlfwOpenGL3>);

TEST(ImGuiBackendContractTest, ProvidesCrossPlatformDefaults) {
    Backend::GlConfig config = Backend::RecommendedGlConfig();
    EXPECT_GE(config.major, 3);
    EXPECT_TRUE(config.coreProfile);
    EXPECT_NE(config.glsl, nullptr);
#if defined(__APPLE__)
    EXPECT_TRUE(config.forwardCompat);
    EXPECT_GE(config.minor, 2);
#endif
}

TEST(ImGuiGlfwPlatformModuleTest, ExposesPlatformApi) {
    Backend::GlfwPlatform::Window* window = nullptr;
    Backend::GlfwPlatform::Monitor* monitor = nullptr;

    auto initForOpenGL = &Backend::GlfwPlatform::InitForOpenGL;
    auto shutdown = &Backend::GlfwPlatform::Shutdown;
    auto newFrame = &Backend::GlfwPlatform::NewFrame;
    auto framebufferSize = &Backend::GlfwPlatform::FramebufferSize;
    auto applyGlConfig = &Backend::GlfwPlatform::ApplyGlConfig;
    auto lastError = &Backend::GlfwPlatform::LastError;

    EXPECT_EQ(window, nullptr);
    EXPECT_EQ(monitor, nullptr);
    EXPECT_NE(initForOpenGL, nullptr);
    EXPECT_NE(shutdown, nullptr);
    EXPECT_NE(newFrame, nullptr);
    EXPECT_NE(framebufferSize, nullptr);
    EXPECT_NE(applyGlConfig, nullptr);
    EXPECT_NE(lastError, nullptr);
}

TEST(ImGuiOpenGL3RendererModuleTest, ExposesRendererApi) {
    auto init = &Backend::OpenGL3Renderer::Init;
    auto shutdown = &Backend::OpenGL3Renderer::Shutdown;
    auto newFrame = &Backend::OpenGL3Renderer::NewFrame;
    auto viewport = &Backend::OpenGL3Renderer::Viewport;
    auto clearColor = &Backend::OpenGL3Renderer::ClearColor;
    auto clearColorBuffer = &Backend::OpenGL3Renderer::ClearColorBuffer;
    auto renderDrawData = &Backend::OpenGL3Renderer::RenderDrawData;

    EXPECT_NE(init, nullptr);
    EXPECT_NE(shutdown, nullptr);
    EXPECT_NE(newFrame, nullptr);
    EXPECT_NE(viewport, nullptr);
    EXPECT_NE(clearColor, nullptr);
    EXPECT_NE(clearColorBuffer, nullptr);
    EXPECT_NE(renderDrawData, nullptr);
}

TEST(ImGuiGlfwOpenGL3BackendModuleTest, ExposesUnifiedApi) {
    auto initGlfw = &Backend::GlfwOpenGL3::InitGlfw;
    auto terminateGlfw = &Backend::GlfwOpenGL3::TerminateGlfw;
    auto createWindow = &Backend::GlfwOpenGL3::CreateWindow;
    auto destroyWindow = &Backend::GlfwOpenGL3::DestroyWindow;
    auto makeContextCurrent = &Backend::GlfwOpenGL3::MakeContextCurrent;
    auto framebufferSize = &Backend::GlfwOpenGL3::FramebufferSize;
    auto init = &Backend::GlfwOpenGL3::Init;
    auto shutdown = &Backend::GlfwOpenGL3::Shutdown;
    auto newFrame = &Backend::GlfwOpenGL3::NewFrame;
    auto viewport = &Backend::GlfwOpenGL3::Viewport;
    auto clearColor = &Backend::GlfwOpenGL3::ClearColor;
    auto clearColorBuffer = &Backend::GlfwOpenGL3::ClearColorBuffer;
    auto renderDrawData = &Backend::GlfwOpenGL3::RenderDrawData;
    auto swapBuffers = &Backend::GlfwOpenGL3::SwapBuffers;
    auto lastError = &Backend::GlfwOpenGL3::LastError;

    EXPECT_NE(initGlfw, nullptr);
    EXPECT_NE(terminateGlfw, nullptr);
    EXPECT_NE(createWindow, nullptr);
    EXPECT_NE(destroyWindow, nullptr);
    EXPECT_NE(makeContextCurrent, nullptr);
    EXPECT_NE(framebufferSize, nullptr);
    EXPECT_NE(init, nullptr);
    EXPECT_NE(shutdown, nullptr);
    EXPECT_NE(newFrame, nullptr);
    EXPECT_NE(viewport, nullptr);
    EXPECT_NE(clearColor, nullptr);
    EXPECT_NE(clearColorBuffer, nullptr);
    EXPECT_NE(renderDrawData, nullptr);
    EXPECT_NE(swapBuffers, nullptr);
    EXPECT_NE(lastError, nullptr);
}
