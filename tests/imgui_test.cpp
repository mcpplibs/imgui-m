#include <gtest/gtest.h>

import std;
import imgui.core;

TEST(ImGuiCoreModuleTest, ExposesVersion) {
    const char* version = ImGui::GetVersion();
    ASSERT_NE(version, nullptr);
    EXPECT_FALSE(std::string_view(version).empty());
}

TEST(ImGuiCoreModuleTest, CreatesAndDestroysContext) {
    ImGuiContext* context = ImGui::CreateContext();
    ASSERT_NE(context, nullptr);

    ImGui::SetCurrentContext(context);
    EXPECT_EQ(ImGui::GetCurrentContext(), context);

    ImGui::DestroyContext(context);
}

TEST(ImGuiCoreModuleTest, ExposesFrameAndWidgetApi) {
    ImGuiContext* context = ImGui::CreateContext();
    ASSERT_NE(context, nullptr);
    ImGui::SetCurrentContext(context);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2 { 320.0f, 240.0f };
    unsigned char* fontPixels = nullptr;
    int fontWidth = 0;
    int fontHeight = 0;
    io.Fonts->GetTexDataAsRGBA32(&fontPixels, &fontWidth, &fontHeight);
    ASSERT_NE(fontPixels, nullptr);
    EXPECT_GT(fontWidth, 0);
    EXPECT_GT(fontHeight, 0);

    ImGui::NewFrame();
    bool open = true;
    EXPECT_TRUE(ImGui::Begin("mcpp imgui", &open));
    EXPECT_FALSE(ImGui::Button("button"));
    ImGui::TextUnformatted("hello");
    ImGui::End();
    ImGui::Render();

    ImDrawData* drawData = ImGui::GetDrawData();
    ASSERT_NE(drawData, nullptr);

    ImGui::DestroyContext(context);
}
