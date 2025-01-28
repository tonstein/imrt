#pragma once

#include <imrt.h>

#include "dsp.h"
#include "params.h"

class Gui : public ImRt::Gui<Gui, Dsp>
{
public:
   Gui(Dsp& dsp, ImRt::GuiSettings settings = ImRt::GuiSettings());

   void onStart();
   void onUpdate();

private:
   ImRt::Knob<Gui, Dsp> gainKnob { *this, dsp, gainId };
   ImRt::Knob<Gui, Dsp> panKnob { *this, dsp, panId };
};
