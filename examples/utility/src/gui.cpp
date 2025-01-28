#include "gui.h"

Gui::Gui(Dsp& dsp, ImRt::GuiSettings settings)
   : ImRt::Gui<Gui, Dsp>(dsp, settings)
{
}

void Gui::onStart() { }

void Gui::onUpdate()
{
   gainKnob.show();
   ImGui::SameLine();
   panKnob.show();
}
