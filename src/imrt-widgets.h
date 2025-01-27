#pragma once

#include "imrt-gui.h"

namespace ImRt {

template <typename Derived, typename Dsp> class Slider {
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
   float _speed = 0.0f;
};

} // namespace ImRt