#include "dsp.h"
#include "imgui.h"
#include "params.h"

Dsp::Dsp(ImRt::DspSettings settings)
   : ImRt::Dsp<Dsp>(settings)
{
   addParameter(gainLayout);
   addParameter(panLayout);
   addParameter(muteLayout);
}

int Dsp::process(ImRt::Buffer& in, ImRt::Buffer& out, uint32_t numFrames)
{
   for (uint32_t frame = 0; frame < numFrames; ++frame)
   {
      updateParameterValue(muteId);
      muteValue = parameterValue(muteId);

      if (muteValue > 0.5)
      {
         out.clear();
         return 0;
      }

      updateParameterValue(gainId);
      gainValue = parameterValue(gainId);

      updateParameterValue(panId);
      panValue = parameterValue(panId);

      if (panValue < 0)
      {
         panAmountL = 1.0f;
         panAmountR = 1.0f + panValue;
      }
      else
      {
         panAmountL = 1.0f - panValue;
         panAmountR = 1.0f;
      }

      out.getSample(0, frame) = in.getSample(0, frame) * panAmountL * gainValue;
      out.getSample(1, frame) = in.getSample(0, frame) * panAmountR * gainValue;
   }

   return 0;
}
