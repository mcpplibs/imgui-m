#include <gtest/gtest.h>

import imgui.core;
import imgui.backend.glfw;
import imgui.backend.opengl3;
import imgui.backend.glfw_opengl3;

TEST(ImGuiGlfwBackendModuleTest, ExposesPlatformApi) {
    ImGui::Backend::Glfw::Window* window = nullptr;
    ImGui::Backend::Glfw::Monitor* monitor = nullptr;

    auto initForOpenGL = &ImGui::Backend::Glfw::InitForOpenGL;
    auto shutdown = &ImGui::Backend::Glfw::Shutdown;
    auto newFrame = &ImGui::Backend::Glfw::NewFrame;
    auto installCallbacks = &ImGui::Backend::Glfw::InstallCallbacks;
    auto restoreCallbacks = &ImGui::Backend::Glfw::RestoreCallbacks;
    auto getContentScaleForWindow = &ImGui::Backend::Glfw::GetContentScaleForWindow;
    auto getContentScaleForMonitor = &ImGui::Backend::Glfw::GetContentScaleForMonitor;
    auto getError = &ImGui::Backend::Glfw::GetError;

    EXPECT_EQ(window, nullptr);
    EXPECT_EQ(monitor, nullptr);
    EXPECT_NE(initForOpenGL, nullptr);
    EXPECT_NE(shutdown, nullptr);
    EXPECT_NE(newFrame, nullptr);
    EXPECT_NE(installCallbacks, nullptr);
    EXPECT_NE(restoreCallbacks, nullptr);
    EXPECT_NE(getContentScaleForWindow, nullptr);
    EXPECT_NE(getContentScaleForMonitor, nullptr);
    EXPECT_NE(getError, nullptr);
}

TEST(ImGuiOpenGL3BackendModuleTest, ExposesRendererApi) {
    auto init = &ImGui::Backend::OpenGL3::Init;
    auto shutdown = &ImGui::Backend::OpenGL3::Shutdown;
    auto newFrame = &ImGui::Backend::OpenGL3::NewFrame;
    auto renderDrawData = &ImGui::Backend::OpenGL3::RenderDrawData;

    EXPECT_NE(init, nullptr);
    EXPECT_NE(shutdown, nullptr);
    EXPECT_NE(newFrame, nullptr);
    EXPECT_NE(renderDrawData, nullptr);
}

TEST(ImGuiGlfwOpenGL3BackendModuleTest, ExposesCombinedApi) {
    auto initGlfw = &ImGui::Backend::GlfwOpenGL3::InitGlfw;
    auto terminateGlfw = &ImGui::Backend::GlfwOpenGL3::TerminateGlfw;
    auto configureOpenGL = &ImGui::Backend::GlfwOpenGL3::ConfigureOpenGL;
    auto setNextWindowVisible = &ImGui::Backend::GlfwOpenGL3::SetNextWindowVisible;
    auto createWindow = &ImGui::Backend::GlfwOpenGL3::CreateWindow;
    auto destroyWindow = &ImGui::Backend::GlfwOpenGL3::DestroyWindow;
    auto makeContextCurrent = &ImGui::Backend::GlfwOpenGL3::MakeContextCurrent;
    auto init = &ImGui::Backend::GlfwOpenGL3::Init;
    auto shutdown = &ImGui::Backend::GlfwOpenGL3::Shutdown;
    auto newFrame = &ImGui::Backend::GlfwOpenGL3::NewFrame;
    auto renderDrawData = &ImGui::Backend::GlfwOpenGL3::RenderDrawData;
    auto getError = &ImGui::Backend::GlfwOpenGL3::GetError;

    EXPECT_NE(initGlfw, nullptr);
    EXPECT_NE(terminateGlfw, nullptr);
    EXPECT_NE(configureOpenGL, nullptr);
    EXPECT_NE(setNextWindowVisible, nullptr);
    EXPECT_NE(createWindow, nullptr);
    EXPECT_NE(destroyWindow, nullptr);
    EXPECT_NE(makeContextCurrent, nullptr);
    EXPECT_NE(init, nullptr);
    EXPECT_NE(shutdown, nullptr);
    EXPECT_NE(newFrame, nullptr);
    EXPECT_NE(renderDrawData, nullptr);
    EXPECT_NE(getError, nullptr);
}
