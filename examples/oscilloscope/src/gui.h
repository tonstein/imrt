#pragma once

#include <imrt.h>

#include "dsp.h"

class Gui : public ImRt::Gui<Gui, Dsp>
{
public:
   Gui(Dsp& dsp, ImRt::GuiSettings settings = ImRt::GuiSettings());

   void onStart();

   void onUpdate();
};
