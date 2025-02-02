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

   _bufferOscilloscope.resize({ 2, _numFramesOscilloscope });
   _bufferOscilloscope.clear();

   _bufferVolume.resize({ 2, _numFramesVolume });
   _bufferVolume.clear();
}

int Dsp::process(ImRt::Buffer& in, ImRt::Buffer& out, uint32_t numFrames)
{
   for (uint32_t frame = 0; frame < numFrames; ++frame)
   {
      _muteValue = updatedParameterValue(_muteId);

      if (_muteValue > 0.5)
      {
         out.clear();
         viewOscilloscope.clear();
         return 0;
      }

      _panValue = updatedParameterValue(_panId);

      if (_panValue < 0)
      {
         _panAmountL = 1.0f;
         _panAmountR = 1.0f + _panValue;
      }
      else
      {
         _panAmountL = 1.0f - _panValue;
         _panAmountR = 1.0f;
      }

      _gainValue = updatedParameterValue(_gainId);

      out.getSample(0, frame)
         = in.getSample(0, frame) * _panAmountL * _gainValue;
      out.getSample(1, frame)
         = in.getSample(1, frame) * _panAmountR * _gainValue;

      _bufferOscilloscope.getSample(0, _posOscilloscope)
         = out.getSample(0, frame);
      _bufferOscilloscope.getSample(1, _posOscilloscope)
         = out.getSample(1, frame);
      _posOscilloscope = (_posOscilloscope + 1) % _numFramesOscilloscope;

      _bufferVolume.getSample(0, _posVolume) = out.getSample(0, frame);
      _bufferVolume.getSample(1, _posVolume) = out.getSample(1, frame);
      _posVolume = (_posVolume + 1) % _numFramesVolume;
   }

   viewVolume       = _bufferVolume;
   viewOscilloscope = _bufferOscilloscope;

   return 0;
}
