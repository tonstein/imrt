#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include <imgui-knobs.h>

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

/* ------------------------------------------------------ */
/*                        VALUE BAR                       */
/* ------------------------------------------------------ */

template <typename Derived, typename Dsp>
class ValueBar
{
public:
   ValueBar(Gui<Derived, Dsp>& gui, const float minValue, const float maxValue)
      : _gui(gui)
      , _min(minValue)
      , _max(maxValue)
      , _difference(maxValue - minValue)
   {
   }

   void show(float value)
   {
      const ImGuiStyle& style  = ImGui::GetStyle();
      ImDrawList* draw_list    = ImGui::GetWindowDrawList();
      const ImVec2& cursor_pos = ImGui::GetCursorScreenPos();
      const float fraction     = std::abs(value - _min) / (_max - _min);

      draw_list->AddRectFilled(
         cursor_pos, cursor_pos + _itemSize,
         ImGui::GetColorU32(ImGuiCol_FrameBg), style.FrameRounding
      );
      draw_list->AddRectFilled(
         cursor_pos + ImVec2 { 0, (1 - fraction) * _itemSize.y },
         cursor_pos + _itemSize, ImGui::GetColorU32(ImGuiCol_PlotHistogram),
         style.FrameRounding, ImDrawFlags_RoundCornersBottom
      );
      ImGui::ItemSize(_itemSize);
   }

protected:
   Gui<Derived, Dsp>& _gui;
   const float _min, _max, _difference;
   const ImVec2 _itemSize = { 20, 200 };
};

/* ------------------------------------------------------ */
/*                       VOLUME BAR                       */
/* ------------------------------------------------------ */

template <typename Derived, typename Dsp>
class VolumeBar : public ValueBar<Derived, Dsp>
{
public:
   VolumeBar(Gui<Derived, Dsp>& gui, ImRt::BufferView& view, uint32_t channel)
      : ImRt::ValueBar<Derived, Dsp>(gui, 0.0f, 1.0f)
      , _view(view)
      , _channel(channel)
   {
   }

   void show()
   {
      for (uint32_t k = 0; k < _view.getNumFrames(); ++k)
      {
         _max = std::max(_max, std::abs((float)_view.getSample(_channel, k)));

         ++_pos;
         if (_pos == _size)
         {
            _pos    = _pos - _size;
            _volume = _max;
            _max    = 0.0f;
         }
      }
      ImRt::ValueBar<Derived, Dsp>::show(_volume);
   }

   float value()
   {
      return std::max(-99.0f, 20 * std::log(_volume));
   }

private:
   ImRt::BufferView& _view;
   const uint32_t _channel;

   float _max { 0.0f }, _volume { 0.0f };
   uint32_t _pos { 0 }, _size { 128 };
};

} // namespace ImRt
