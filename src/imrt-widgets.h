#pragma once

#include <imgui-knobs.h>

#include "imrt-gui.h"

namespace ImRt {

/* ------------------------------------------------------ */
/*                         slider                         */
/* ------------------------------------------------------ */

template <typename Derived, typename Dsp>
class Slider
{
public:
   Slider(Gui<Derived, Dsp>& gui, Dsp& dsp, uint32_t paramId)
      : _gui(gui)
      , _dsp(dsp)
      , _paramId(paramId)
   {
   }

   void show()
   {
      auto p = _gui.parameters.byId(_paramId);
      if (ImGui::SliderFloat(p->name(), &p->value, p->min(), p->max()))
         _dsp.pushParameter(_paramId, p->value);
   }

private:
   Gui<Derived, Dsp>& _gui;
   Dsp& _dsp;
   const uint32_t _paramId;
};

/* ------------------------------------------------------ */
/*                          knob                          */
/* ------------------------------------------------------ */

template <typename Derived, typename Dsp>
class Knob
{
public:
   Knob(Gui<Derived, Dsp>& gui, Dsp& dsp, uint32_t paramId)
      : _gui(gui)
      , _dsp(dsp)
      , _paramId(paramId)
   {
   }

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

         _dsp.pushParameter(_paramId, p->value);
      }

      if (ImGui::IsItemActive() && ImGui::IsMouseDoubleClicked(0))
      {
         p->value = p->init();
         _dsp.pushParameter(_paramId, p->value);
      }
   }

private:
   Gui<Derived, Dsp>& _gui;
   Dsp& _dsp;
   const uint32_t _paramId;
   float _speed = 0.0f;
};

} // namespace ImRt