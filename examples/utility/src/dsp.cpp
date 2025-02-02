#include "dsp.h"
#include "params.h"
#include <cstdint>

Dsp::Dsp(ImRt::DspSettings settings)
   : ImRt::Dsp<Dsp>(settings)
   , _gainId(gainLayout.id())
   , _panId(panLayout.id())
   , _muteId(muteLayout.id())
{
   addParameter(gainLayout);
   addParameter(panLayout);
   addParameter(muteLayout);

   _oscBuffer.resize({ 2, _oscNumFrames });
   _oscBuffer.clear();

   _volBuffer.resize({ 2, _volNumFrames });
   _volBuffer.clear();
}

int Dsp::process(ImRt::Buffer& in, ImRt::Buffer& out, uint32_t numFrames)
{
   for (uint32_t frame = 0; frame < numFrames; ++frame)
   {
      float muteValue = updatedParameterValue(_muteId);

      if (muteValue > 0.5)
      {
         out.clear();
         volView.clear();
         oscView.clear();
         return 0;
      }

      float panValue   = updatedParameterValue(_panId);
      float panAmountL = 1.0f - std::max(0.0f, panValue);
      float panAmountR = 1.0f + std::min(0.0f, panValue);

      float _gainValue = updatedParameterValue(_gainId);

      out.getSample(0, frame)
         = in.getSample(0, frame) * panAmountL * _gainValue;
      out.getSample(1, frame)
         = in.getSample(1, frame) * panAmountR * _gainValue;

      _oscBuffer.getSample(0, _oscPos) = out.getSample(0, frame);
      _oscBuffer.getSample(1, _oscPos) = out.getSample(1, frame);
      _oscPos                          = (_oscPos + 1) % _oscNumFrames;

      _volBuffer.getSample(0, _volPos) = out.getSample(0, frame);
      _volBuffer.getSample(1, _volPos) = out.getSample(1, frame);
      _volPos                          = (_volPos + 1) % _volNumFrames;
   }

   volView = _volBuffer;
   oscView = _oscBuffer;

   return 0;
}
