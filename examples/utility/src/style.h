#pragma once

#include <imrt.h>

// clang-format off
namespace ImRt {
namespace Color {

constexpr ImVec4 White100   = ImColor(212, 212, 212).Value;
constexpr ImVec4 Cyan100    = ImColor(141, 200, 255).Value;
constexpr ImVec4 Magenta100 = ImColor(255, 155, 203).Value;
constexpr ImVec4 Yellow100  = ImColor(255, 206, 87).Value;
constexpr ImVec4 Green100   = ImColor(117, 196, 121).Value;

// Derived colors
constexpr ImVec4 Black      = ImVec4(White100.x * 0.08f, White100.y * 0.08f, White100.z * 0.08f, 1.0f);
constexpr ImVec4 Gray12     = ImVec4(White100.x * 0.12f, White100.y * 0.12f, White100.z * 0.12f, 1.0f);
constexpr ImVec4 Gray16     = ImVec4(White100.x * 0.16f, White100.y * 0.16f, White100.z * 0.16f, 1.0f);
constexpr ImVec4 Gray19     = ImVec4(White100.x * 0.19f, White100.y * 0.19f, White100.z * 0.19f, 1.0f);
constexpr ImVec4 Cyan10     = ImVec4(Cyan100.x  * 0.1f,  Cyan100.y  * 0.1f,  Cyan100.z  * 0.1f,  1.0f);
constexpr ImVec4 MagentaA30 = ImVec4(Magenta100.x, Magenta100.y, Magenta100.z, 0.3f);
constexpr ImVec4 CyanA30    = ImVec4(Cyan100.x, Cyan100.y, Cyan100.z, 0.3f);

} // namespace Color
// clang-format on

static ImRt::Style DefaultStyle()
{
   ImRt::Style style;

   style.gui.WindowBorderSize       = 0;
   style.gui.GrabMinSize            = 40;
   style.gui.ItemInnerSpacing       = { 8, 8 };
   style.gui.ItemSpacing            = { 8, 8 };
   style.gui.WindowRounding         = 1;
   style.gui.ChildRounding          = 1;
   style.gui.FrameRounding          = 1;
   style.gui.GrabRounding           = 1;
   style.gui.PopupRounding          = 1;
   style.gui.ScrollbarRounding      = 1;
   style.gui.TabRounding            = 1;
   style.gui.AntiAliasedLines       = true;
   style.gui.AntiAliasedLinesUseTex = true;
   style.gui.AntiAliasedFill        = true;

   style.gui.Colors[ImGuiCol_Text]             = ImRt::Color::White100;
   style.gui.Colors[ImGuiCol_WindowBg]         = ImRt::Color::Gray12;
   style.gui.Colors[ImGuiCol_TitleBg]          = ImRt::Color::Gray16;
   style.gui.Colors[ImGuiCol_TitleBgCollapsed] = ImRt::Color::Gray16;
   style.gui.Colors[ImGuiCol_TitleBgActive]    = ImRt::Color::Black;
   style.gui.Colors[ImGuiCol_ResizeGrip]       = ImRt::Color::Gray16;
   style.gui.Colors[ImGuiCol_FrameBg]          = ImRt::Color::Black;
   style.gui.Colors[ImGuiCol_FrameBgHovered]   = ImRt::Color::Black;
   style.gui.Colors[ImGuiCol_FrameBgActive]    = ImRt::Color::Black;
   style.gui.Colors[ImGuiCol_SliderGrab]       = ImRt::Color::Gray16;
   style.gui.Colors[ImGuiCol_SliderGrabActive] = ImRt::Color::Gray19;
   style.gui.Colors[ImGuiCol_Button]           = ImRt::Color::White100;
   style.gui.Colors[ImGuiCol_ButtonHovered]    = ImRt::Color::White100;
   style.gui.Colors[ImGuiCol_ButtonActive]     = ImRt::Color::White100;
   style.gui.Colors[ImGuiCol_CheckMark]        = ImRt::Color::Green100;

   style.plot.PlotBorderSize = 0;

   style.plot.Colors[ImPlotCol_Line]       = ImRt::Color::Magenta100;
   style.plot.Colors[ImPlotCol_Fill]       = ImRt::Color::MagentaA30;
   style.plot.Colors[ImPlotCol_PlotBg]     = ImRt::Color::Cyan10;
   style.plot.Colors[ImPlotCol_Crosshairs] = ImRt::Color::Yellow100;
   style.plot.Colors[ImPlotCol_InlayText]  = ImRt::Color::Yellow100;

   return style;
}
} // namespace ImRt
