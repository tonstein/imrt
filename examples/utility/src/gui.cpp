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
   , _volumeBarL(*this, dsp.view, 0)
   , _volumeBarR(*this, dsp.view, 1)

{
}

void Gui::onStart() { }

void Gui::onUpdate()
{
   if (ImGui::BeginChild("#Left", { 235, 0 }))
   {
      _gainKnob.show();

      ImGui::SameLine();
      _panKnob.show();

      _muteButton.show();
   }
   ImGui::EndChild();

   ImGui::SameLine();

   if (ImGui::BeginChild("#Right", { 235, 0 }))
   {
      _volumeBarL.show();

      ImGui::SameLine();
      _volumeBarR.show();

      ImGui::SameLine();
      showVolumeLabels();
   }
   ImGui::EndChild();
}

void Gui::showVolumeLabels()
{
   if (ImGui::BeginChild("#Volume Labels", { 235, 0 }))
   {
      float volumeLdB = _volumeBarL.value();
      if (volumeLdB < -90.0f)
      {
         ImGui::Text("L: -inf dB");
      }
      else
      {
         ImGui::Text("L: %.1f dB", volumeLdB);
      }

      float volumeRdB = _volumeBarR.value();
      if (volumeRdB < -90.0f)
      {
         ImGui::Text("R: -inf dB");
      }
      else
      {
         ImGui::Text("R: %.1f dB", volumeRdB);
      }
   }
   ImGui::EndChild();
}
