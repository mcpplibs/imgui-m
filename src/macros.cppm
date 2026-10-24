module;
#include <imgui.h>
namespace mwrap_imgui_capture {
inline constexpr auto value_IMGUI_VERSION = IMGUI_VERSION;
inline constexpr auto value_IMGUI_VERSION_NUM = IMGUI_VERSION_NUM;
}
#undef IMGUI_VERSION
#undef IMGUI_VERSION_NUM

export module mcpplibs.mwrap.imgui.macros;

export namespace mcpplibs::mwrap::imgui::macros {
inline constexpr auto IMGUI_VERSION = mwrap_imgui_capture::value_IMGUI_VERSION;
inline constexpr auto IMGUI_VERSION_NUM = mwrap_imgui_capture::value_IMGUI_VERSION_NUM;

}
