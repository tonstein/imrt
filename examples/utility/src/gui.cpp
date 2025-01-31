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
      showVolumeBars();
   }
   ImGui::EndChild();
}

void Gui::showVolumeBars()
{
   for (uint32_t k = 0; k < dsp.view.getNumFrames(); ++k)
   {
      maxL = std::max(maxL, std::abs((float)dsp.view.getSample(0, k)));
      maxR = std::max(maxR, std::abs((float)dsp.view.getSample(1, k)));

      ++bufferPos;

      if (bufferPos == bufferSize)
      {
         bufferPos -= bufferSize;

         volumeL = maxL;
         volumeR = maxR;

         maxL = 0.0f;
         maxR = 0.0f;
      }
   }

   volumeBarL.show(volumeL);

   ImGui::SameLine();
   volumeBarR.show(volumeR);

   ImGui::SameLine();
   if (ImGui::BeginChild("#Volume Labels", { 150, 200 }))
   {
      float volumeLdB = std::max(-99.0f, 20 * std::log(volumeL));
      float volumeRdB = std::max(-99.0f, 20 * std::log(volumeR));

      if (volumeLdB < -90.0f)
      {
         ImGui::Text("L: -inf dB");
      }
      else
      {
         ImGui::Text("L: %.1f dB", volumeLdB);
      }

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
