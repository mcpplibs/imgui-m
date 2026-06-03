export module imgui.backend.glfw_opengl3;

import imgui.core;
export import imgui.backend; // shared types (GlConfig/Error/FbSize) are part of the backend surface
import imgui.backend.platform.glfw;
import imgui.backend.renderer.opengl3;

// Concrete backend: assembles the GLFW platform piece and the OpenGL3 renderer
// piece into a single Backend type that satisfies BackendApi. Consumer code is
// identical across backends; swapping backend is a different import + alias.
//
// This module imports imgui.core for signatures but does NOT re-export it.
export namespace ImGui::Backend {
    struct GlfwOpenGL3 {
        using Window = GlfwPlatform::Window;
        using Monitor = GlfwPlatform::Monitor;

        // Platform-neutral contract names; the Glfw-flavored spellings below
        // are kept as aliases for existing consumers.
        static bool InitPlatform() {
            return GlfwPlatform::InitGlfw();
        }

        static void TerminatePlatform() {
            GlfwPlatform::TerminateGlfw();
        }

        static bool InitGlfw() {
            return InitPlatform();
        }

        static void TerminateGlfw() {
            TerminatePlatform();
        }

        static const char* VersionString() {
            return GlfwPlatform::VersionString();
        }

        static Error LastError() {
            return GlfwPlatform::LastError();
        }

        // Cross-platform window creation: applies default hints + GlConfig
        // (incl. macOS forward-compat) before creating the window.
        static Window* CreateWindow(
            int width,
            int height,
            const char* title,
            GlConfig config = RecommendedGlConfig()
        ) {
            GlfwPlatform::DefaultWindowHints();
            GlfwPlatform::ApplyGlConfig(config);
            return GlfwPlatform::CreateWindow(width, height, title);
        }

        static void DestroyWindow(Window* window) {
            GlfwPlatform::DestroyWindow(window);
        }

        static void MakeContextCurrent(Window* window) {
            GlfwPlatform::MakeContextCurrent(window);
        }

        static void SwapInterval(int interval) {
            GlfwPlatform::SwapInterval(interval);
        }

        static FbSize FramebufferSize(Window* window) {
            return GlfwPlatform::FramebufferSize(window);
        }

        static bool WindowShouldClose(Window* window) {
            return GlfwPlatform::WindowShouldClose(window);
        }

        static void SetWindowShouldClose(Window* window, bool value) {
            GlfwPlatform::SetWindowShouldClose(window, value);
        }

        static void PollEvents() {
            GlfwPlatform::PollEvents();
        }

        static void SwapBuffers(Window* window) {
            GlfwPlatform::SwapBuffers(window);
        }

        // Initialize ImGui platform + renderer bindings. glsl is taken from the
        // same config used for window creation (default RecommendedGlConfig()).
        static bool Init(
            Window* window,
            GlConfig config = RecommendedGlConfig(),
            bool installCallbacks = true
        ) {
            if (!GlfwPlatform::InitForOpenGL(window, installCallbacks)) {
                return false;
            }
            if (!OpenGL3Renderer::Init(config.glsl)) {
                GlfwPlatform::Shutdown();
                return false;
            }
            return true;
        }

        static void NewFrame() {
            OpenGL3Renderer::NewFrame();
            GlfwPlatform::NewFrame();
        }

        static void Viewport(int x, int y, int width, int height) {
            OpenGL3Renderer::Viewport(x, y, width, height);
        }

        static void ClearColor(float red, float green, float blue, float alpha) {
            OpenGL3Renderer::ClearColor(red, green, blue, alpha);
        }

        static void ClearColorBuffer() {
            OpenGL3Renderer::ClearColorBuffer();
        }

        static void RenderDrawData(ImDrawData* drawData) {
            OpenGL3Renderer::RenderDrawData(drawData);
        }

        static void ShutdownRenderer() {
            OpenGL3Renderer::Shutdown();
        }

        static void ShutdownPlatform() {
            GlfwPlatform::Shutdown();
        }

        static void Shutdown() {
            ShutdownRenderer();
            ShutdownPlatform();
        }

        static void InstallCallbacks(Window* window) {
            GlfwPlatform::InstallCallbacks(window);
        }

        static void RestoreCallbacks(Window* window) {
            GlfwPlatform::RestoreCallbacks(window);
        }

        static void SetCallbacksChainForAllWindows(bool chainForAllWindows) {
            GlfwPlatform::SetCallbacksChainForAllWindows(chainForAllWindows);
        }
    };

    static_assert(BackendApi<GlfwOpenGL3>, "GlfwOpenGL3 must satisfy the backend contract");
}
