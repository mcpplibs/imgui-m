import std;
import imgui.core;
import imgui.backend.glfw_opengl3;

// Swap the import above and this alias to switch backends; the rest is identical.
using Backend = ImGui::Backend::GlfwOpenGL3;

namespace {
    int fail(const char* step, int exitCode, ImGui::Backend::Error error) {
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
    if (!Backend::InitGlfw()) {
        return fail("glfwInit", 1, Backend::LastError());
    }

    auto* window = Backend::CreateWindow(640, 360, "mcpp imgui minimal window");
    if (window == nullptr) {
        const auto error = Backend::LastError();
        Backend::TerminateGlfw();
        return fail("glfwCreateWindow", 2, error);
    }

    Backend::MakeContextCurrent(window);
    Backend::SwapInterval(1);

    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);

    if (!Backend::Init(window)) {
        const auto error = Backend::LastError();
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

        const auto framebuffer = Backend::FramebufferSize(window);
        Backend::Viewport(0, 0, framebuffer.width, framebuffer.height);
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
