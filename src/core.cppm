module;

#include <imgui.h>

export module imgui.core;

export using ImDrawData = ::ImDrawData;
export using ImGuiContext = ::ImGuiContext;
export using ImGuiIO = ::ImGuiIO;
export using ImFontAtlas = ::ImFontAtlas;
export using ImVec2 = ::ImVec2;
export using ImVec4 = ::ImVec4;

export namespace ImGui {
    using ::ImGui::Begin;
    using ::ImGui::Button;
    using ::ImGui::CreateContext;
    using ::ImGui::DestroyContext;
    using ::ImGui::End;
    using ::ImGui::GetCurrentContext;
    using ::ImGui::GetDrawData;
    using ::ImGui::GetIO;
    using ::ImGui::GetVersion;
    using ::ImGui::NewFrame;
    using ::ImGui::Render;
    using ::ImGui::SetCurrentContext;
    using ::ImGui::TextUnformatted;
}
