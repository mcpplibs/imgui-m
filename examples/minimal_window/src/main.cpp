import std;
import imgui.backend.glfw_opengl3;

namespace {
    struct GlfwError {
        int code = 0;
        const char* description = nullptr;
    };

    GlfwError captureError() {
        const char* description = nullptr;
        const int errorCode = ImGui::Backend::GlfwOpenGL3::GetError(&description);
        return GlfwError{errorCode, description};
    }

    int fail(const char* step, int exitCode, GlfwError error) {
        std::println(
            "failed at {}: GLFW error {} ({})",
            step,
            error.code,
            error.description != nullptr ? error.description : "no GLFW error description"
        );
        return exitCode;
    }
}

int main() {
    namespace Backend = ImGui::Backend::GlfwOpenGL3;

    if (!Backend::InitGlfw()) {
        return fail("glfwInit", 1, captureError());
    }

    Backend::DefaultWindowHints();
    Backend::ConfigureOpenGL(3, 3, true, false);

    auto* window = Backend::CreateWindow(640, 360, "mcpp imgui minimal window");
    if (window == nullptr) {
        const auto error = captureError();
        Backend::TerminateGlfw();
        return fail("glfwCreateWindow", 2, error);
    }

    Backend::MakeContextCurrent(window);
    Backend::SwapInterval(1);

    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);

    if (!Backend::Init(window)) {
        const auto error = captureError();
        ImGui::DestroyContext(context);
        Backend::DestroyWindow(window);
        Backend::TerminateGlfw();
        return fail("ImGui GLFW/OpenGL3 backend init", 3, error);
    }

    while (!Backend::WindowShouldClose(window)) {
        Backend::PollEvents();
        Backend::NewFrame();

        ImGui::NewFrame();
        ImGui::Begin("Minimal ImGui Window");
        ImGui::TextUnformatted("mcpp module import, no include in consumer code");
        ImGui::End();
        ImGui::Render();

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        Backend::GetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        Backend::Viewport(0, 0, framebufferWidth, framebufferHeight);
        Backend::ClearColor(0.08f, 0.09f, 0.10f, 1.0f);
        Backend::ClearColorBuffer();
        Backend::RenderDrawData(ImGui::GetDrawData());
        Backend::SwapBuffers(window);
    }

    Backend::Shutdown();
    ImGui::DestroyContext(context);
    Backend::DestroyWindow(window);
    Backend::TerminateGlfw();
    return 0;
}
