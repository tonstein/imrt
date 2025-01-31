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

   void showVolumeLabels();

private:
   ImRt::Knob<Gui, Dsp> gainKnob { *this, gainId };
   ImRt::Knob<Gui, Dsp> panKnob { *this, panId };
   ImRt::ToggleButton<Gui, Dsp> muteButton { *this, muteId };
   ImRt::VolumeBar<Gui, Dsp> volumeBarL { *this, dsp.view, 0 };
   ImRt::VolumeBar<Gui, Dsp> volumeBarR { *this, dsp.view, 1 };
};
