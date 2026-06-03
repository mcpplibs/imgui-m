export module imgui.backend;

import imgui.core;

// Generic backend abstraction layer.
//
// This module owns the shared value types, the cross-platform default GL
// configuration, and the compile-time API contract every concrete backend
// must satisfy. It contains no platform code and does not re-export
// imgui.core: consumers import imgui.core themselves.
export namespace ImGui::Backend {
    // Requested OpenGL context configuration. Defaults are filled by
    // RecommendedGlConfig() so examples never hard-code platform values.
    struct GlConfig {
        int major = 3;
        int minor = 3;
        bool coreProfile = true;
        bool forwardCompat = false;
        const char* glsl = nullptr; // nullptr => let the renderer pick a default
    };

    // Last platform error, surfaced for diagnostics.
    struct Error {
        int code = 0;
        const char* description = nullptr;
    };

    // Framebuffer size in pixels.
    struct FbSize {
        int width = 0;
        int height = 0;
    };

    // Platform-correct context/shader defaults.
    //   macOS  : GL 3.2 core + forward-compat, "#version 150"
    //   others : GL 3.3 core, "#version 130"
    GlConfig RecommendedGlConfig() {
#if defined(__APPLE__)
        return GlConfig{3, 2, true, true, "#version 150"};
#else
        return GlConfig{3, 3, true, false, "#version 130"};
#endif
    }

    // Compile-time contract. Every backend exposes one type T whose static
    // surface satisfies this concept, so consumer code is identical across
    // backends and a backend swap is just a different import + alias.
    template <class T>
    concept BackendApi = requires (typename T::Window* window, ImDrawData* drawData, GlConfig config) {
        typename T::Window;
        T::InitGlfw();
        T::TerminateGlfw();
        T::CreateWindow(0, 0, "");
        T::DestroyWindow(window);
        T::MakeContextCurrent(window);
        T::Init(window, config);
        T::NewFrame();
        T::Viewport(0, 0, 0, 0);
        T::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        T::ClearColorBuffer();
        T::RenderDrawData(drawData);
        T::SwapBuffers(window);
        T::PollEvents();
        T::WindowShouldClose(window);
        T::FramebufferSize(window);
        T::Shutdown();
        T::LastError();
    };
}
