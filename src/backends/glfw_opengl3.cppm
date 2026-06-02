export module imgui.backend.glfw_opengl3;

export import imgui.core;
export import imgui.backend.glfw;
export import imgui.backend.opengl3;

export namespace ImGui::Backend::GlfwOpenGL3 {
    using Monitor = Glfw::Monitor;
    using Window = Glfw::Window;

    bool InitGlfw() {
        return Glfw::InitGlfw();
    }

    void TerminateGlfw() {
        Glfw::TerminateGlfw();
    }

    const char* GlfwVersionString() {
        return Glfw::GlfwVersionString();
    }

    void DefaultWindowHints() {
        Glfw::DefaultWindowHints();
    }

    void ConfigureOpenGL(int major, int minor, bool coreProfile = true, bool forwardCompat = false) {
        Glfw::ConfigureOpenGL(major, minor, coreProfile, forwardCompat);
    }

    void SetNextWindowVisible(bool visible) {
        Glfw::SetNextWindowVisible(visible);
    }

    Window* CreateWindow(
        int width,
        int height,
        const char* title,
        Monitor* monitor = nullptr,
        Window* share = nullptr
    ) {
        return Glfw::CreateWindow(width, height, title, monitor, share);
    }

    void DestroyWindow(Window* window) {
        Glfw::DestroyWindow(window);
    }

    void MakeContextCurrent(Window* window) {
        Glfw::MakeContextCurrent(window);
    }

    void SwapInterval(int interval) {
        Glfw::SwapInterval(interval);
    }

    bool WindowShouldClose(Window* window) {
        return Glfw::WindowShouldClose(window);
    }

    void SetWindowShouldClose(Window* window, bool value) {
        Glfw::SetWindowShouldClose(window, value);
    }

    void PollEvents() {
        Glfw::PollEvents();
    }

    void SwapBuffers(Window* window) {
        Glfw::SwapBuffers(window);
    }

    bool InitForOpenGL(Window* window, bool installCallbacks = true) {
        return Glfw::InitForOpenGL(window, installCallbacks);
    }

    bool InitRenderer(const char* glslVersion = nullptr) {
        return OpenGL3::Init(glslVersion);
    }

    bool Init(Window* window, const char* glslVersion = nullptr, bool installCallbacks = true) {
        if (!InitForOpenGL(window, installCallbacks)) {
            return false;
        }
        if (!InitRenderer(glslVersion)) {
            Glfw::Shutdown();
            return false;
        }
        return true;
    }

    void ShutdownRenderer() {
        OpenGL3::Shutdown();
    }

    void ShutdownPlatform() {
        Glfw::Shutdown();
    }

    void Shutdown() {
        ShutdownRenderer();
        ShutdownPlatform();
    }

    void NewFrame() {
        OpenGL3::NewFrame();
        Glfw::NewFrame();
    }

    void RenderDrawData(ImDrawData* drawData) {
        OpenGL3::RenderDrawData(drawData);
    }

    void InstallCallbacks(Window* window) {
        Glfw::InstallCallbacks(window);
    }

    void RestoreCallbacks(Window* window) {
        Glfw::RestoreCallbacks(window);
    }

    void SetCallbacksChainForAllWindows(bool chainForAllWindows) {
        Glfw::SetCallbacksChainForAllWindows(chainForAllWindows);
    }
}
