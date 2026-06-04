export module imgui.app;

import std;                          // private: std::println for init diagnostics
export import imgui.core;            // consumers get the ImGui::* surface for free
import imgui.backend.glfw_opengl3;   // private: the default concrete backend

// Tier-0 convenience facade.
//
// `imgui.app` owns the entire window/context lifecycle so a consumer only has to
// supply a per-frame UI callback. It is deliberately a header-style inline
// template in the module interface: the UI callback type is a template
// parameter, so no <functional> dependency is needed and the call is fully
// inlined. For more control, drop down to the Tier-1 `imgui.backend` contract or
// the Tier-2 explicit `imgui.backend.<impl>` module.
export namespace ImGui::App {
    struct Options {
        int width = 1280;
        int height = 720;
        const char* title = "mcpp imgui app";
    };

    // Runs the full GLFW/OpenGL3 lifecycle, calling `ui()` once per frame.
    // Returns 0 on a clean exit, nonzero on an initialization failure (the
    // backend's last error is printed in that case).
    template <class Ui>
    int run(Options opts, Ui&& ui) {
        using Backend = ImGui::Backend::GlfwOpenGL3;

        if (!Backend::InitGlfw()) {
            const auto error = Backend::LastError();
            std::println(
                "imgui.app: glfwInit failed: GLFW error {} ({})",
                error.code,
                error.description != nullptr ? error.description : "no description"
            );
            return 1;
        }

        auto* window = Backend::CreateWindow(opts.width, opts.height, opts.title);
        if (window == nullptr) {
            const auto error = Backend::LastError();
            std::println(
                "imgui.app: window creation failed: GLFW error {} ({})",
                error.code,
                error.description != nullptr ? error.description : "no description"
            );
            Backend::TerminateGlfw();
            return 2;
        }

        Backend::MakeContextCurrent(window);
        Backend::SwapInterval(1);

        ImGuiContext* context = ImGui::CreateContext();
        ImGui::SetCurrentContext(context);
#ifdef MCPP_FEATURE_DOCKING
        // `docking` feature: enable dockable windows out of the box.
        ImGui::GetIO().ConfigFlags |= ImGui::ConfigFlags_DockingEnable;
#endif
#ifdef MCPP_FEATURE_VIEWPORTS
        // `viewports` feature: windows dragged outside the main window
        // detach into real OS windows.
        ImGui::GetIO().ConfigFlags |= ImGui::ConfigFlags_ViewportsEnable;
#endif

        if (!Backend::Init(window)) {
            const auto error = Backend::LastError();
            std::println(
                "imgui.app: backend init failed: GLFW error {} ({})",
                error.code,
                error.description != nullptr ? error.description : "no description"
            );
            ImGui::DestroyContext(context);
            Backend::DestroyWindow(window);
            Backend::TerminateGlfw();
            return 3;
        }

        while (!Backend::WindowShouldClose(window)) {
            Backend::PollEvents();
            Backend::NewFrame();

            ImGui::NewFrame();
            ui();
            ImGui::Render();

            const auto framebuffer = Backend::FramebufferSize(window);
            Backend::Viewport(0, 0, framebuffer.width, framebuffer.height);
            Backend::ClearColor(0.10f, 0.10f, 0.12f, 1.0f);
            Backend::ClearColorBuffer();
            Backend::RenderDrawData(ImGui::GetDrawData());
#ifdef MCPP_FEATURE_VIEWPORTS
            // Render the detached OS windows, then restore the main context.
            {
                auto* backup = Backend::GetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                Backend::MakeContextCurrent(backup);
            }
#endif
            Backend::SwapBuffers(window);
        }

        Backend::Shutdown();
        ImGui::DestroyContext(context);
        Backend::DestroyWindow(window);
        Backend::TerminateGlfw();
        return 0;
    }

    // Convenience overload using default Options.
    template <class Ui>
    int run(Ui&& ui) {
        return run(Options{}, static_cast<Ui&&>(ui));
    }
}
