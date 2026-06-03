module;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

export module imgui.backend.glfw;

import imgui.core;

export namespace ImGui::Backend::Glfw {
    using Monitor = ::GLFWmonitor;
    using Window = ::GLFWwindow;

    bool InitGlfw() {
        return glfwInit() == GLFW_TRUE;
    }

    void TerminateGlfw() {
        glfwTerminate();
    }

    const char* GlfwVersionString() {
        return glfwGetVersionString();
    }

    int GetError(const char** description) {
        return glfwGetError(description);
    }

    void DefaultWindowHints() {
        glfwDefaultWindowHints();
    }

    void ConfigureOpenGL(int major, int minor, bool coreProfile = true, bool forwardCompat = false) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
        if (coreProfile) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forwardCompat ? GLFW_TRUE : GLFW_FALSE);
    }

    void SetNextWindowVisible(bool visible) {
        glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);
    }

    Window* CreateWindow(
        int width,
        int height,
        const char* title,
        Monitor* monitor = nullptr,
        Window* share = nullptr
    ) {
        return glfwCreateWindow(width, height, title, monitor, share);
    }

    void DestroyWindow(Window* window) {
        glfwDestroyWindow(window);
    }

    void MakeContextCurrent(Window* window) {
        glfwMakeContextCurrent(window);
    }

    void GetFramebufferSize(Window* window, int* width, int* height) {
        glfwGetFramebufferSize(window, width, height);
    }

    void SwapInterval(int interval) {
        glfwSwapInterval(interval);
    }

    bool WindowShouldClose(Window* window) {
        return glfwWindowShouldClose(window) == GLFW_TRUE;
    }

    void SetWindowShouldClose(Window* window, bool value) {
        glfwSetWindowShouldClose(window, value ? GLFW_TRUE : GLFW_FALSE);
    }

    void PollEvents() {
        glfwPollEvents();
    }

    void SwapBuffers(Window* window) {
        glfwSwapBuffers(window);
    }

    bool InitForOpenGL(Window* window, bool installCallbacks = true) {
        return ImGui_ImplGlfw_InitForOpenGL(window, installCallbacks);
    }

    void Shutdown() {
        ImGui_ImplGlfw_Shutdown();
    }

    void NewFrame() {
        ImGui_ImplGlfw_NewFrame();
    }

    void InstallCallbacks(Window* window) {
        ImGui_ImplGlfw_InstallCallbacks(window);
    }

    void RestoreCallbacks(Window* window) {
        ImGui_ImplGlfw_RestoreCallbacks(window);
    }

    void SetCallbacksChainForAllWindows(bool chainForAllWindows) {
        ImGui_ImplGlfw_SetCallbacksChainForAllWindows(chainForAllWindows);
    }

    void WindowFocusCallback(Window* window, int focused) {
        ImGui_ImplGlfw_WindowFocusCallback(window, focused);
    }

    void CursorEnterCallback(Window* window, int entered) {
        ImGui_ImplGlfw_CursorEnterCallback(window, entered);
    }

    void CursorPosCallback(Window* window, double x, double y) {
        ImGui_ImplGlfw_CursorPosCallback(window, x, y);
    }

    void MouseButtonCallback(Window* window, int button, int action, int mods) {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    }

    void ScrollCallback(Window* window, double xoffset, double yoffset) {
        ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    }

    void KeyCallback(Window* window, int key, int scancode, int action, int mods) {
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    }

    void CharCallback(Window* window, unsigned int c) {
        ImGui_ImplGlfw_CharCallback(window, c);
    }

    void MonitorCallback(Monitor* monitor, int event) {
        ImGui_ImplGlfw_MonitorCallback(monitor, event);
    }

    void Sleep(int milliseconds) {
        ImGui_ImplGlfw_Sleep(milliseconds);
    }

    float GetContentScaleForWindow(Window* window) {
        return ImGui_ImplGlfw_GetContentScaleForWindow(window);
    }

    float GetContentScaleForMonitor(Monitor* monitor) {
        return ImGui_ImplGlfw_GetContentScaleForMonitor(monitor);
    }
}
