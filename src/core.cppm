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

// Docking surface — gated by the `docking` feature
// (`imgui = { ..., features = ["docking"] }`). Requires the upstream docking
// sources (compat.imgui 1.92.8-docking); with the feature off, the docking
// sources behave exactly like mainline.
#ifdef MCPP_FEATURE_DOCKING
// (ImGuiID / ImGuiDockNodeFlags are plain-integer typedefs; GMF typedefs
//  cannot be re-aliased under the same global name in a module. Consumers
//  take DockSpace* return values with `auto`.)
export namespace ImGui {
    using ::ImGui::DockSpace;
    using ::ImGui::DockSpaceOverViewport;
    using ::ImGui::SetNextWindowDockID;
    using ::ImGui::GetWindowDockID;
    using ::ImGui::IsWindowDocked;

    // Unscoped global enumerators, re-exported as namespaced constants for
    // module consumers.
    inline constexpr int ConfigFlags_DockingEnable = ::ImGuiConfigFlags_DockingEnable;
    inline constexpr int Cond_FirstUseEver         = ::ImGuiCond_FirstUseEver;
}
#endif
