#pragma once

#include <imrt.h>

#include "dsp.h"

class Gui : public ImRt::Gui<Gui, Dsp>
{
public:
   Gui(Dsp& dsp, ImRt::GuiSettings settings = ImRt::GuiSettings());

   void onStart();
   void onUpdate();

   void showVolumeLabels();

private:
   uint32_t _gainId, _panId, _muteId;
   ImRt::Knob<Gui, Dsp> _gainKnob, _panKnob;
   ImRt::ToggleButton<Gui, Dsp> _muteButton;
   ImRt::VolumeBar<Gui, Dsp> _volumeBarL, _volumeBarR;
};
