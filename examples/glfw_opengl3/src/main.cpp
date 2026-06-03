import std;
import imgui.core;
import imgui.backend.glfw_opengl3;

// Swap the import above and this alias to switch backends; the rest is identical.
using Backend = ImGui::Backend::GlfwOpenGL3;

int main() {
    if (!Backend::InitGlfw()) {
        std::println("failed to initialize GLFW");
        return 1;
    }

    auto* window = Backend::CreateWindow(960, 540, "mcpp imgui glfw/opengl3 demo");
    if (window == nullptr) {
        std::println("failed to create GLFW window");
        Backend::TerminateGlfw();
        return 1;
    }

    Backend::MakeContextCurrent(window);
    Backend::SwapInterval(1);

    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);

    if (!Backend::Init(window)) {
        std::println("failed to initialize ImGui GLFW/OpenGL3 backends");
        ImGui::DestroyContext(context);
        Backend::DestroyWindow(window);
        Backend::TerminateGlfw();
        return 1;
    }

    while (!Backend::WindowShouldClose(window)) {
        Backend::PollEvents();
        Backend::NewFrame();

        ImGui::NewFrame();
        bool open = true;
        ImGui::Begin("mcpp imgui", &open);
        ImGui::TextUnformatted("Hello from import imgui.backend.glfw_opengl3");
        if (ImGui::Button("Close")) {
            Backend::SetWindowShouldClose(window, true);
        }
        ImGui::End();
        ImGui::Render();

        const auto framebuffer = Backend::FramebufferSize(window);
        Backend::Viewport(0, 0, framebuffer.width, framebuffer.height);
        Backend::ClearColor(0.08f, 0.09f, 0.10f, 1.0f);
        Backend::ClearColorBuffer();
        Backend::RenderDrawData(ImGui::GetDrawData());
        Backend::SwapBuffers(window);

        if (!open) {
            Backend::SetWindowShouldClose(window, true);
        }
    }

    Backend::Shutdown();
    ImGui::DestroyContext(context);
    Backend::DestroyWindow(window);
    Backend::TerminateGlfw();
    return 0;
}
