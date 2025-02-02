#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include <cstdint>
#include <imgui-knobs.h>
#include "implot.h"

#include "imrt-gui.h"
#include "imrt-constants.h"

namespace ImRt {

/* -------------------------------------------------------------------------- */
/*                      TOGGLE BUTTON                                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief A toggle button that controls a GUI parameter value and announces the
 * changes to the corresponding DSP parameter value automatically.
 */

template <typename Derived, typename Dsp>
class ToggleButton
{
public:
   /**
    * @brief Constructs a new ToggleButton object that can be painted onto the
    * given Gui object. It automatically controls the GuiParameter with the
    * given parameter ID and announces the value changes to the corresponding
    * DspParameter.
    */
   ToggleButton(Gui<Derived, Dsp>& gui, uint32_t paramId)
      : _gui(gui)
      , _paramId(paramId)
      , _param(_gui.parameters.byId(_paramId))
   {
   }

   /**
    * @brief Paints the togggle button using ImGui at the current ImGui
    * CursorPos.
    */
   void show()
   {
      _buttonState = (_param->value > 0.5) ? true : false;
      if (ImGui::RadioButton(_param->name(), _buttonState == true))
      {
         _buttonState = !_buttonState;

         _param->value = _buttonState ? 1.0f : 0.0f;
         _gui.dsp.announceParameterChange(_paramId, _param->value);
      }
   }

private:
   Gui<Derived, Dsp>& _gui;
   const uint32_t _paramId;
   GuiParameter* _param;
   bool _buttonState;
};

/* -------------------------------------------------------------------------- */
/*                         SLIDER                                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief A slider for a parameter that controls a GUI parameter value and
 * announces the changes to the corresponding DSP parameter value automatically.
 */
template <typename Derived, typename Dsp>
class Slider
{
public:
   /**
    * @brief Constructs a new Slider object that can be painted onto the given
    * Gui object. It automatically controls the GuiParameter with the given
    * parameter ID and announces the value changes to the corresponding
    * DspParameter.
    */
   Slider(Gui<Derived, Dsp>& gui, uint32_t paramId)
      : _gui(gui)
      , _paramId(paramId)
      , _param(_gui.parameters.byId(_paramId))
   {
   }

   /**
    * @brief Paints the slider using ImGui at the current ImGui CursorPos.
    */
   void show()
   {
      if (ImGui::SliderFloat(
             _param->name(), &_param->value, _param->min(), _param->max()
          ))
      {
         _gui.dsp.announce(_paramId, _param->value);
      }
   }

private:
   Gui<Derived, Dsp>& _gui;
   const uint32_t _paramId;
   GuiParameter* _param;
};

/* -------------------------------------------------------------------------- */
/*                          KNOB                                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief A knob for a parameter that controls a GUI parameter value and
 * announces the changes to the corresponding DSP parameter value automatically.
 */
template <typename Derived, typename Dsp>
class Knob
{
public:
   /**
    * @brief Constructs a new Knob object that can be painted onto the given
    * Gui object. It automatically controls the GuiParameter with the given
    * parameter ID and announces the value changes to the corresponding
    * DspParameter.
    */
   Knob(Gui<Derived, Dsp>& gui, uint32_t paramId)
      : _gui(gui)
      , _paramId(paramId)
      , _param(_gui.parameters.byId(paramId))
   {
      _knobFlags = ImGuiKnobFlags_AlwaysClamp;
      // knobFlags |= ImGuiKnobFlags_ValueTooltip
      // knobFlags |= ImGuiKnobFlags_NoInput;
      // knobFlags |= ImGuiKnobFlags_Logarithmic;
   }

   /**
    * @brief Paints the knob using ImGui at the current ImGui CursorPos.
    *
    */
   void show()
   {
      ImGui::GetCursorPos();
      if (ImGuiKnobs::Knob(
             _param->name(), &_param->value, _param->min(), _param->max(),
             _speed, "%.3f", ImGuiKnobVariant_Dot, 100, _knobFlags
          ))
      {
         ImGui::GetIO().KeyCtrl
            ? _speed = (_param->max() - _param->min()) / 1000
            : _speed = (_param->max() - _param->min()) / 200;

         _gui.dsp.announceParameterChange(_paramId, _param->value);
      }

      if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
      {
         _param->value = _param->init();
         _gui.dsp.announceParameterChange(_paramId, _param->value);
      }
   }

private:
   Gui<Derived, Dsp>& _gui;
   const uint32_t _paramId;
   GuiParameter* _param;

   ImGuiKnobFlags _knobFlags;
   float _speed = 0.0f;
};

/* -------------------------------------------------------------------------- */
/*                        VALUE BAR                                           */
/* -------------------------------------------------------------------------- */

template <typename Derived, typename Dsp>
class ValueBar
{
public:
   ValueBar(
      Gui<Derived, Dsp>& gui, const float minValue, const float maxValue,
      const ImVec2 widgetSize = { 15, 200 }
   )
      : _gui(gui)
      , _min(minValue)
      , _max(maxValue)
      , _difference(maxValue - minValue)
      , _widgetSize(widgetSize)
   {
   }

   void show(float value)
   {
      const ImGuiStyle& style = ImGui::GetStyle();
      ImDrawList* drawList    = ImGui::GetWindowDrawList();
      const ImVec2& cursorPos = ImGui::GetCursorScreenPos();
      const float fraction    = std::abs(value - _min) / (_max - _min);

      drawList->AddRectFilled(
         cursorPos, cursorPos + _widgetSize,
         ImGui::GetColorU32(ImGuiCol_FrameBg), style.FrameRounding
      );
      drawList->AddRectFilled(
         cursorPos + ImVec2 { 0, (1 - fraction) * _widgetSize.y },
         cursorPos + _widgetSize, ImGui::GetColorU32(ImGuiCol_PlotHistogram),
         style.FrameRounding, ImDrawFlags_RoundCornersBottom
      );

      auto colorTop = ImGui::GetColorU32(
         ImGui::GetStyleColorVec4(ImGuiCol_FrameBg)
         * ImVec4(1.0f, 1.0f, 1.0f, 0.0f)
      );
      auto colorBottom = ImGui::GetColorU32(
         ImGui::GetStyleColorVec4(ImGuiCol_FrameBg)
         * ImVec4(1.0f, 1.0f, 1.0f, 0.9f)
      );

      drawList->AddRectFilledMultiColor(
         cursorPos, cursorPos + _widgetSize, colorTop, colorTop, colorBottom,
         colorBottom
      );
      ImGui::ItemSize(_widgetSize);
   }

protected:
   Gui<Derived, Dsp>& _gui;
   const float _min, _max, _difference;
   ImVec2 _widgetSize;
};

/* -------------------------------------------------------------------------- */
/*                       VOLUME BAR                                           */
/* -------------------------------------------------------------------------- */

template <typename Derived, typename Dsp>
class VolumeBar : public ValueBar<Derived, Dsp>
{
public:
   VolumeBar(
      Gui<Derived, Dsp>& gui, ImRt::BufferView& view, uint32_t channel,
      ImVec2 itemSize = { 15, 200 }
   )
      : ImRt::ValueBar<Derived, Dsp>(gui, -72.0f, 0.0f, itemSize)
      , _view(view)
      , _channel(channel)
   {
   }

   void show()
   {
      float volume       = 0.0f;
      uint32_t numFrames = _view.getNumFrames();

      for (uint32_t frame = 0; frame < numFrames; ++frame)
      {
         float newVolume = std::abs(_view.getSample(_channel, frame));
         volume          = std::max(volume, newVolume);
      }
      ImRt::ValueBar<Derived, Dsp>::show(20.0f * std::log(volume));
   }

private:
   ImRt::BufferView& _view;
   const uint32_t _channel;
};

/* -------------------------------------------------------------------------- */
/*                      OSCILLOSCOPE                                          */
/* -------------------------------------------------------------------------- */

template <typename Derived, typename Dsp>
class Oscilloscope
{
public:
   Oscilloscope(
      Gui<Derived, Dsp>& gui, ImRt::BufferView& view,
      ImVec2 widgetSize = { 300, 200 }
   )
      : _view(view)
      , _widgetSize(widgetSize)
   {
   }

   void show()
   {
      uint32_t numChannels = std::min<uint32_t>(2, _view.getNumChannels());
      uint32_t numFrames   = _view.getNumFrames();

      ImPlotFlags plotFlags = ImPlotFlags_CanvasOnly;
      ImPlotAxisFlags axisFlags
         = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Lock;

      bool plotted = ImPlot::BeginPlot("#NoTitle", _widgetSize, plotFlags);

      ImPlot::SetupAxes(nullptr, nullptr, axisFlags, axisFlags);
      ImPlot::SetupAxisLimits(ImAxis_X1, 0, numFrames - 1);
      ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);

      ImVec4 color[2];
      color[0] = ImPlot::GetStyle().Colors[ImPlotCol_Line];
      color[1] = color[0] * ImVec4(0.0f, 1.0f, 1.0f, 1.0f);

      for (uint32_t channel = 0; channel < numChannels; ++channel)
      {
         ImPlot::PushStyleColor(ImPlotCol_Line, color[channel]);
         ImPlot::PlotLine("", &_view.getSample(channel, 0), numFrames);
         ImPlot::PopStyleColor();
      }

      if (plotted)
      {
         ImPlot::EndPlot();
      }
   }

private:
   ImVec2 _widgetSize;
   ImRt::BufferView& _view;
};

} // namespace ImRt
