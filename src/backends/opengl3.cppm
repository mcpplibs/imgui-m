module;

#include <imgui_impl_opengl3.h>

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

    void RenderDrawData(ImDrawData* drawData) {
        ImGui_ImplOpenGL3_RenderDrawData(drawData);
    }
}
