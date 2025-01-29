#pragma once

#include <imgui-knobs.h>

#include "imgui.h"
#include "imrt-gui.h"

namespace ImRt {

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

         _gui.dsp.announce(_paramId, _param->value);
      }

      if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
      {
         _param->value = _param->init();
         _gui.dsp.announce(_paramId, _param->value);
      }
   }

private:
   Gui<Derived, Dsp>& _gui;
   const uint32_t _paramId;
   GuiParameter* _param;

   ImGuiKnobFlags _knobFlags;
   float _speed = 0.0f;
};

} // namespace ImRt
