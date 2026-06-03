module;

#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

export module imgui.backend.opengl3;

import imgui.core;

export namespace ImGui::Backend::OpenGL3 {
    bool Init(const char* glslVersion = nullptr) {
        return ImGui_ImplOpenGL3_Init(glslVersion);
    }

    void Shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
    }

    void NewFrame() {
        ImGui_ImplOpenGL3_NewFrame();
    }

    void Viewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void ClearColor(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha);
    }

    void ClearColorBuffer() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderDrawData(ImDrawData* drawData) {
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
    }
}
