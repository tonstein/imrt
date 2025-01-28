#pragma once

#include <imgui-knobs.h>

#include "imrt-gui.h"

namespace ImRt {

/* -------------------------------------------------------------------------- */
/*                         SLIDER                                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief A template Slider class for a parameter that controls a GUI parameter
 * value and announces the changes to the corresponding DSP parameter value
 * automatically.
 *
 * @tparam Derived A class derived from GUI<>.
 * @tparam Dsp A class derived from Dsp<>.
 */
template <typename Derived, typename Dsp>
class Slider
{
public:
   /**
    * @brief Construct a new Slider object that can be painted onto the given
    * Gui object. It controls the GuiParameter with the given parameter ID and
    * announces the value changes to the corresponding DspParameter of the given
    * Dsp object.
    */
   Slider(Gui<Derived, Dsp>& gui, Dsp& dsp, uint32_t paramId)
      : _gui(gui)
      , _dsp(dsp)
      , _paramId(paramId)
   {
   }

   /**
    * @brief Paints the slider using ImGui.
    */
   void show()
   {
      auto p = _gui.parameters.byId(_paramId);
      if (ImGui::SliderFloat(p->name(), &p->value, p->min(), p->max()))
      {
         _dsp.announce(_paramId, p->value);
      }
   }

private:
   Gui<Derived, Dsp>& _gui;
   Dsp& _dsp;
   const uint32_t _paramId;
};

/* -------------------------------------------------------------------------- */
/*                          KNOB                                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief A template Knob class for a parameter that controls a GUI parameter
 * value and announces the changes to the corresponding DSP parameter value
 * automatically.
 *
 * @tparam Derived A class derived from GUI<>.
 * @tparam Dsp A class derived from Dsp<>.
 */
template <typename Derived, typename Dsp>
class Knob
{
public:
   /**
    * @brief Construct a new Knob object that can be painted onto the given
    * Gui object. It controls the GuiParameter with the given parameter ID and
    * announces the value changes to the corresponding DspParameter of the given
    * Dsp object.
    */
   Knob(Gui<Derived, Dsp>& gui, Dsp& dsp, uint32_t paramId)
      : _gui(gui)
      , _dsp(dsp)
      , _paramId(paramId)
   {
   }

   /**
    * @brief Paints the knob using ImGui.
    *
    */
   void show()
   {
      auto p = _gui.parameters.byId(_paramId);

      ImGuiKnobFlags knobFlags = ImGuiKnobFlags_AlwaysClamp;
      // knobFlags |= ImGuiKnobFlags_ValueTooltip
      // knobFlags |= ImGuiKnobFlags_NoInput;
      // knobFlags |= ImGuiKnobFlags_Logarithmic;

      if (ImGuiKnobs::Knob(
             p->name(), &p->value, p->min(), p->max(), _speed, "%.3f",
             ImGuiKnobVariant_Dot, 100, knobFlags
          ))
      {
         ImGui::GetIO().KeyCtrl ? _speed = (p->max() - p->min()) / 1000
                                : _speed = (p->max() - p->min()) / 200;

         _dsp.announce(_paramId, p->value);
      }

      if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
      {
         p->value = p->init();
         _dsp.announce(_paramId, p->value);
      }
   }

private:
   Gui<Derived, Dsp>& _gui;
   Dsp& _dsp;
   const uint32_t _paramId;
   float _speed = 0.0f;
};

} // namespace ImRt
