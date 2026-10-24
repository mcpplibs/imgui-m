import mcpplibs.mwrap.imgui;
namespace macros = mcpplibs::mwrap::imgui::macros;
int main() { static_assert(macros::IMGUI_VERSION_NUM == 19280); return macros::IMGUI_VERSION[0] != '1'; }
