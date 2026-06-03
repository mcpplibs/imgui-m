module;

#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

export module imgui.backend.renderer.opengl3;

import imgui.core;
import imgui.backend;

// OpenGL3 renderer piece: GL framebuffer ops plus the ImGui_ImplOpenGL3_*
// renderer binding. Reusable assembly unit; composed into a concrete backend
// by imgui.backend.glfw_opengl3. Does not re-export imgui.core.
//
// The renderer deliberately does not clear implicitly in RenderDrawData so an
// application can render its own scene first and draw ImGui as an overlay.
export namespace ImGui::Backend {
    struct OpenGL3Renderer {
        static bool Init(const char* glslVersion = nullptr) {
            return ImGui_ImplOpenGL3_Init(glslVersion);
        }

        static void Shutdown() {
            ImGui_ImplOpenGL3_Shutdown();
        }

        static void NewFrame() {
            ImGui_ImplOpenGL3_NewFrame();
        }

        static void Viewport(int x, int y, int width, int height) {
            glViewport(x, y, width, height);
        }

        static void ClearColor(float red, float green, float blue, float alpha) {
            glClearColor(red, green, blue, alpha);
        }

        static void ClearColorBuffer() {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        static void RenderDrawData(ImDrawData* drawData) {
            ImGui_ImplOpenGL3_RenderDrawData(drawData);
        }
    };
}
