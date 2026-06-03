module;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

export module imgui.backend.platform.glfw;

import imgui.core;
import imgui.backend;

// GLFW platform piece: window/event management plus the ImGui_ImplGlfw_*
// platform binding. Reusable assembly unit; composed into a concrete backend
// by imgui.backend.glfw_opengl3. Does not re-export imgui.core.
export namespace ImGui::Backend {
    struct GlfwPlatform {
        using Window = ::GLFWwindow;
        using Monitor = ::GLFWmonitor;

        static bool InitGlfw() {
            return glfwInit() == GLFW_TRUE;
        }

        static void TerminateGlfw() {
            glfwTerminate();
        }

        static const char* VersionString() {
            return glfwGetVersionString();
        }

        static Error LastError() {
            const char* description = nullptr;
            const int code = glfwGetError(&description);
            return Error{code, description};
        }

        static void DefaultWindowHints() {
            glfwDefaultWindowHints();
        }

        // Apply context/profile hints. Must be called before CreateWindow.
        static void ApplyGlConfig(const GlConfig& config) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config.major);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config.minor);
            if (config.coreProfile) {
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            }
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, config.forwardCompat ? GLFW_TRUE : GLFW_FALSE);
        }

        static void SetNextWindowVisible(bool visible) {
            glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);
        }

        static Window* CreateWindow(
            int width,
            int height,
            const char* title,
            Monitor* monitor = nullptr,
            Window* share = nullptr
        ) {
            return glfwCreateWindow(width, height, title, monitor, share);
        }

        static void DestroyWindow(Window* window) {
            glfwDestroyWindow(window);
        }

        static void MakeContextCurrent(Window* window) {
            glfwMakeContextCurrent(window);
        }

        static FbSize FramebufferSize(Window* window) {
            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            return FbSize{width, height};
        }

        static void SwapInterval(int interval) {
            glfwSwapInterval(interval);
        }

        static bool WindowShouldClose(Window* window) {
            return glfwWindowShouldClose(window) == GLFW_TRUE;
        }

        static void SetWindowShouldClose(Window* window, bool value) {
            glfwSetWindowShouldClose(window, value ? GLFW_TRUE : GLFW_FALSE);
        }

        static void PollEvents() {
            glfwPollEvents();
        }

        static void SwapBuffers(Window* window) {
            glfwSwapBuffers(window);
        }

        // ImGui platform binding.
        static bool InitForOpenGL(Window* window, bool installCallbacks = true) {
            return ImGui_ImplGlfw_InitForOpenGL(window, installCallbacks);
        }

        static void Shutdown() {
            ImGui_ImplGlfw_Shutdown();
        }

        static void NewFrame() {
            ImGui_ImplGlfw_NewFrame();
        }

        static void InstallCallbacks(Window* window) {
            ImGui_ImplGlfw_InstallCallbacks(window);
        }

        static void RestoreCallbacks(Window* window) {
            ImGui_ImplGlfw_RestoreCallbacks(window);
        }

        static void SetCallbacksChainForAllWindows(bool chainForAllWindows) {
            ImGui_ImplGlfw_SetCallbacksChainForAllWindows(chainForAllWindows);
        }

        static float GetContentScaleForWindow(Window* window) {
            return ImGui_ImplGlfw_GetContentScaleForWindow(window);
        }

        static float GetContentScaleForMonitor(Monitor* monitor) {
            return ImGui_ImplGlfw_GetContentScaleForMonitor(monitor);
        }

        static void Sleep(int milliseconds) {
            ImGui_ImplGlfw_Sleep(milliseconds);
        }
    };
}
