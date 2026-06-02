import std;
import imgui.core;
import imgui.backend.glfw_opengl3;

int main() {
    if (!ImGui::Backend::GlfwOpenGL3::InitGlfw()) {
        std::println("failed to initialize GLFW");
        return 1;
    }

    ImGui::Backend::GlfwOpenGL3::DefaultWindowHints();
    ImGui::Backend::GlfwOpenGL3::ConfigureOpenGL(3, 3, true, false);

    auto* window = ImGui::Backend::GlfwOpenGL3::CreateWindow(
        960,
        540,
        "mcpp imgui glfw/opengl3 demo"
    );
    if (window == nullptr) {
        std::println("failed to create GLFW window");
        ImGui::Backend::GlfwOpenGL3::TerminateGlfw();
        return 1;
    }

    ImGui::Backend::GlfwOpenGL3::MakeContextCurrent(window);
    ImGui::Backend::GlfwOpenGL3::SwapInterval(1);

    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);

    if (!ImGui::Backend::GlfwOpenGL3::Init(window)) {
        std::println("failed to initialize ImGui GLFW/OpenGL3 backends");
        ImGui::DestroyContext(context);
        ImGui::Backend::GlfwOpenGL3::DestroyWindow(window);
        ImGui::Backend::GlfwOpenGL3::TerminateGlfw();
        return 1;
    }

    while (!ImGui::Backend::GlfwOpenGL3::WindowShouldClose(window)) {
        ImGui::Backend::GlfwOpenGL3::PollEvents();
        ImGui::Backend::GlfwOpenGL3::NewFrame();

        ImGui::NewFrame();
        bool open = true;
        ImGui::Begin("mcpp imgui", &open);
        ImGui::TextUnformatted("Hello from import imgui.backend.glfw_opengl3");
        if (ImGui::Button("Close")) {
            ImGui::Backend::GlfwOpenGL3::SetWindowShouldClose(window, true);
        }
        ImGui::End();
        ImGui::Render();

        ImGui::Backend::GlfwOpenGL3::RenderDrawData(ImGui::GetDrawData());
        ImGui::Backend::GlfwOpenGL3::SwapBuffers(window);

        if (!open) {
            ImGui::Backend::GlfwOpenGL3::SetWindowShouldClose(window, true);
        }
    }

    ImGui::Backend::GlfwOpenGL3::Shutdown();
    ImGui::DestroyContext(context);
    ImGui::Backend::GlfwOpenGL3::DestroyWindow(window);
    ImGui::Backend::GlfwOpenGL3::TerminateGlfw();
    return 0;
}
