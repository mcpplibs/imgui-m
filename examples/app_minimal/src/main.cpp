import imgui.core;
import imgui.app;

// Tier-0: imgui.app owns the whole lifecycle; we only supply the per-frame UI.
int main() {
    return ImGui::App::run([] {
        ImGui::Begin("hi");
        ImGui::TextUnformatted("imgui.app facade");
        ImGui::End();
    });
}
