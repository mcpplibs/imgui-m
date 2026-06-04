// Docking demo — `imgui = { ..., features = ["docking"] }`.
// The imgui.app facade auto-enables ImGuiConfigFlags_DockingEnable when the
// feature is active; DockSpaceOverViewport turns the whole window into a
// dock host, so the two panels below can be dragged into / split around it.
import imgui.core;
import imgui.app;

int main() {
    return ImGui::App::run({.title = "mcpp imgui docking demo"}, [] {
        const auto dockspace = ImGui::DockSpaceOverViewport();

        // Pre-dock both panels into the dockspace on first run; they remain
        // freely draggable/splittable afterwards.
        ImGui::SetNextWindowDockID(dockspace, ImGui::Cond_FirstUseEver);
        ImGui::Begin("Toolbox");
        ImGui::TextUnformatted("Drag this panel onto the dockspace.");
        ImGui::TextUnformatted("Docked: ");
        ImGui::TextUnformatted(ImGui::IsWindowDocked() ? "yes" : "no");
        ImGui::End();

        ImGui::SetNextWindowDockID(dockspace, ImGui::Cond_FirstUseEver);
        ImGui::Begin("Inspector");
        ImGui::TextUnformatted("Split me against the Toolbox.");
        ImGui::End();
    });
}
