export module imgui.backend.headless;

import imgui.core;
export import imgui.backend; // shared types (GlConfig/Error/FbSize) + BackendApi

// Headless backend: a second, display-free BackendApi implementation.
//
// It satisfies the exact same compile-time contract as GlfwOpenGL3, so the
// SAME application loop runs against either backend by swapping one import
// and one alias (I3). Useful for CI smoke runs, logic tests, and servers:
// frames are produced (ImGui draw data is generated) but nothing is
// presented; RenderDrawData is a deliberate no-op.
//
// This module imports imgui.core for signatures but does NOT re-export it.
export namespace ImGui::Backend {
    struct Headless {
        struct WindowImpl {
            int  width = 0;
            int  height = 0;
            bool shouldClose = false;
        };
        using Window = WindowImpl;

        static bool InitPlatform() { return true; }
        static void TerminatePlatform() {}

        static Error LastError() { return Error{}; }

        static Window* CreateWindow(
            int width,
            int height,
            const char* /*title*/,
            GlConfig /*config*/ = RecommendedGlConfig()
        ) {
            return new WindowImpl{width, height, false};
        }

        static void DestroyWindow(Window* window) { delete window; }

        static void MakeContextCurrent(Window* /*window*/) {}
        static void SwapInterval(int /*interval*/) {}

        static FbSize FramebufferSize(Window* window) {
            return window ? FbSize{window->width, window->height} : FbSize{};
        }

        static bool WindowShouldClose(Window* window) {
            return window == nullptr || window->shouldClose;
        }

        static void SetWindowShouldClose(Window* window, bool value) {
            if (window) window->shouldClose = value;
        }

        static void PollEvents() {}
        static void SwapBuffers(Window* /*window*/) {}

        // ImGui bindings: a headless frame needs a display size and a built
        // font atlas; there is no platform/renderer library to initialize.
        static bool Init(
            Window* window,
            GlConfig /*config*/ = RecommendedGlConfig(),
            bool /*installCallbacks*/ = true
        ) {
            if (window == nullptr || ImGui::GetCurrentContext() == nullptr) return false;
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2{static_cast<float>(window->width),
                                    static_cast<float>(window->height)};
            unsigned char* pixels = nullptr;
            int w = 0, h = 0;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);
            return pixels != nullptr;
        }

        static void NewFrame() {}
        static void Viewport(int, int, int, int) {}
        static void ClearColor(float, float, float, float) {}
        static void ClearColorBuffer() {}
        static void RenderDrawData(ImDrawData* /*drawData*/) {} // headless: no-op
        static void Shutdown() {}
    };

    static_assert(BackendApi<Headless>,
                  "Headless must satisfy the backend contract");
}
