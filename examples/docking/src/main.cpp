// Docking + multi-viewport demo —
//   imgui = { ..., features = ["docking", "viewports"] }
//
// On the first frame, DockBuilder splits the dockspace into an IDE-style
// layout (Scene | Viewport | Inspector, Console at the bottom). Every pane is
// a real dock node: drag any tab to re-split, stack, or float it.
// With `viewports`, drag a tab OUTSIDE the main window and it detaches into
// its own OS window; the "Detached" panel below starts outside on purpose.
import imgui.core;
import imgui.app;

int main() {
    return ImGui::App::run({.title = "mcpp imgui docking demo"}, [] {
        auto dockspace = ImGui::DockSpaceOverViewport();

        static bool layout_built = false;
        if (!layout_built) {
            layout_built = true;
            const auto root = dockspace;
            auto left  = ImGui::DockBuilderSplitNode(
                dockspace, ImGui::Dir_Left, 0.22f, nullptr, &dockspace);
            auto down  = ImGui::DockBuilderSplitNode(
                dockspace, ImGui::Dir_Down, 0.28f, nullptr, &dockspace);
            auto right = ImGui::DockBuilderSplitNode(
                dockspace, ImGui::Dir_Right, 0.30f, nullptr, &dockspace);
            ImGui::DockBuilderDockWindow("Scene",     left);
            ImGui::DockBuilderDockWindow("Console",   down);
            ImGui::DockBuilderDockWindow("Inspector", right);
            ImGui::DockBuilderDockWindow("Viewport",  dockspace);
            ImGui::DockBuilderFinish(root);
        }

        ImGui::Begin("Scene");
        ImGui::TextUnformatted("Scene tree pane (left split).");
        ImGui::TextUnformatted(ImGui::IsWindowDocked() ? "docked: yes" : "docked: no");
        ImGui::End();

        ImGui::Begin("Viewport");
        ImGui::TextUnformatted("Central pane. Drag any tab to");
        ImGui::TextUnformatted("re-split / stack / float it.");
        ImGui::End();

        ImGui::Begin("Inspector");
        ImGui::TextUnformatted("Properties pane (right split).");
        ImGui::End();

        ImGui::Begin("Console");
        ImGui::TextUnformatted("Log pane (bottom split).");
        ImGui::End();

        // Starts OUTSIDE the main window: with `viewports` this is a real,
        // separate OS window from the very first frame.
        const auto* main_vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(
            ImVec2{main_vp->Pos.x + main_vp->Size.x + 60.0f, main_vp->Pos.y + 80.0f},
            ImGui::Cond_FirstUseEver);
        ImGui::Begin("Detached");
        ImGui::TextUnformatted("I live in my own OS window.");
        ImGui::TextUnformatted("Drag me back inside to re-dock.");
        ImGui::End();
    });
}
