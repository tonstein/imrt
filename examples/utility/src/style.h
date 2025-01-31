#pragma once

#include "imgui.h"
#include <imrt.h>

namespace ImRt {
namespace Color {

   static const ImVec4 Black   = ImColor(17, 17, 17).Value;
   static const ImVec4 White   = ImColor(212, 212, 212).Value;
   static const ImVec4 Blue    = ImColor(141, 200, 255).Value;
   static const ImVec4 Magenta = ImColor(255, 155, 203).Value;
   static const ImVec4 Yellow  = ImColor(255, 206, 87).Value;
   static const ImVec4 Green   = ImColor(117, 196, 121).Value;

   // Scalars
   static const ImVec4 Dark        = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
   static const ImVec4 Light       = ImVec4(1.45f, 1.45f, 1.45f, 1.0f);
   static const ImVec4 Transparent = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);

   // Derived Colors [FixMe] Make sure not to scale to a value > 1.
   static const ImVec4 DarkGray           = Black * Light;
   static const ImVec4 Gray               = DarkGray * Light;
   static const ImVec4 DarkBlue           = Blue * Dark;
   static const ImVec4 TransparentMagenta = Magenta * Transparent;
   static const ImVec4 TransparentCyan    = Blue * Transparent;

} // namespace Color

static ImRt::Style DefaultStyle()
{
   ImRt::Style style;

   // ImGui style
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

   ImVec4* colors = style.gui.Colors;

   colors[ImGuiCol_Text]             = ImRt::Color::White;
   colors[ImGuiCol_WindowBg]         = ImRt::Color::DarkGray;
   colors[ImGuiCol_FrameBg]          = ImRt::Color::Black;
   colors[ImGuiCol_FrameBgHovered]   = ImRt::Color::Black;
   colors[ImGuiCol_FrameBgActive]    = ImRt::Color::Black;
   colors[ImGuiCol_SliderGrab]       = ImRt::Color::Gray;
   colors[ImGuiCol_SliderGrabActive] = ImRt::Color::Gray;
   colors[ImGuiCol_CheckMark]        = ImRt::Color::Green;
   colors[ImGuiCol_Button]           = ImRt::Color::White;
   colors[ImGuiCol_ButtonHovered]    = ImRt::Color::White;
   colors[ImGuiCol_ButtonActive]     = ImRt::Color::White;
   colors[ImGuiCol_CheckMark]        = ImRt::Color::White;
   colors[ImGuiCol_PlotHistogram]    = ImRt::Color::Magenta;

   // ImPlot style
   style.plot.PlotBorderSize = 0;

   colors = style.plot.Colors;

   colors[ImPlotCol_Line]       = ImRt::Color::Magenta;
   colors[ImPlotCol_Fill]       = ImRt::Color::TransparentMagenta;
   colors[ImPlotCol_PlotBg]     = ImRt::Color::DarkBlue;
   colors[ImPlotCol_Crosshairs] = ImRt::Color::Yellow;
   colors[ImPlotCol_InlayText]  = ImRt::Color::Yellow;

   return style;
}

} // namespace ImRt
