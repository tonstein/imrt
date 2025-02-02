#include "gui.h"
#include "imgui.h"
#include "params.h"

Gui::Gui(Dsp& dsp, ImRt::GuiSettings settings)
   : ImRt::Gui<Gui, Dsp>(dsp, settings)
   , _gainId(gainLayout.id())
   , _panId(panLayout.id())
   , _muteId(muteLayout.id())
   , _gainKnob(*this, _gainId)
   , _panKnob(*this, _panId)
   , _muteButton(*this, _muteId)
   , _volumeBarL(*this, dsp.volView, 0, { 15, 230 })
   , _volumeBarR(*this, dsp.volView, 1, { 15, 230 })
   , oscilloscope(*this, dsp.oscView, { 870, 230 })
{
}

void Gui::onStart() { }

void Gui::onUpdate()
{
   if (ImGui::BeginChild("#1", { 235, 0 }))
   {
      _gainKnob.show();

      ImGui::SameLine();
      _panKnob.show();

      _muteButton.show();
   }
   ImGui::EndChild();

   ImGui::SameLine();

   if (ImGui::BeginChild("#2", { 46, 0 }))
   {
      _volumeBarL.show();

      ImGui::SameLine();
      _volumeBarR.show();
   }
   ImGui::EndChild();

   ImGui::SameLine();
   oscilloscope.show();
}
