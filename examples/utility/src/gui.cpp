#include "gui.h"
#include "imgui.h"

Gui::Gui(Dsp& dsp, ImRt::GuiSettings settings)
   : ImRt::Gui<Gui, Dsp>(dsp, settings)
{
}

void Gui::onStart() { }

void Gui::onUpdate()
{
   if (ImGui::BeginChild("#Left", { 235, 0 }))
   {
      gainKnob.show();

      ImGui::SameLine();
      panKnob.show();

      muteButton.show();
   }
   ImGui::EndChild();

   ImGui::SameLine();

   if (ImGui::BeginChild("#Right", { 235, 0 }))
   {
      volumeBarL.show();

      ImGui::SameLine();
      volumeBarR.show();

      ImGui::SameLine();
      showVolumeLabels();
   }
   ImGui::EndChild();
}

void Gui::showVolumeLabels()
{
   if (ImGui::BeginChild("#Volume Labels", { 235, 0 }))
   {
      float volumeLdB = volumeBarL.value();
      if (volumeLdB < -90.0f)
      {
         ImGui::Text("L: -inf dB");
      }
      else
      {
         ImGui::Text("L: %.1f dB", volumeLdB);
      }

      float volumeRdB = volumeBarR.value();
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
