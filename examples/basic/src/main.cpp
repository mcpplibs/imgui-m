import std;
import imgui.core;

int main() {
    ImGuiContext* context = ImGui::CreateContext();
    ImGui::SetCurrentContext(context);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2 { 320.0f, 240.0f };
    unsigned char* fontPixels = nullptr;
    int fontWidth = 0;
    int fontHeight = 0;
    io.Fonts->GetTexDataAsRGBA32(&fontPixels, &fontWidth, &fontHeight);

    ImGui::NewFrame();
    bool open = true;
    ImGui::Begin("mcpp imgui", &open);
    ImGui::TextUnformatted("hello from import imgui.core");
    ImGui::End();
    ImGui::Render();

    std::println("Dear ImGui {} module frame ok", ImGui::GetVersion());
    ImGui::DestroyContext(context);
    return 0;
}
