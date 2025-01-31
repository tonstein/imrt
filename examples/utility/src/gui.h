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

   void showVolumeBars();

private:
   ImRt::Knob<Gui, Dsp> gainKnob { *this, gainId };
   ImRt::Knob<Gui, Dsp> panKnob { *this, panId };
   ImRt::ToggleButton<Gui, Dsp> muteButton { *this, muteId };

   ImRt::ValueBar<Gui, Dsp> volumeBarL { *this, 0.0f, 1.0f };
   float maxL    = 0.0f;
   float volumeL = 0.0f;

   ImRt::ValueBar<Gui, Dsp> volumeBarR { *this, 0.0f, 1.0f };
   float maxR    = 0.0f;
   float volumeR = 0.0f;

   uint32_t bufferPos  = 0;
   uint32_t bufferSize = 128;
};
