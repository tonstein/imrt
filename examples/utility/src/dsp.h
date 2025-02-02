#pragma once

#include <cstdint>
#include <imrt.h>

class Dsp : public ImRt::Dsp<Dsp>
{
public:
   Dsp(ImRt::DspSettings settings = ImRt::DspSettings());

   int process(ImRt::Buffer& in, ImRt::Buffer& out, uint32_t numFrames);

   ImRt::BufferView viewOscilloscope, viewVolume;

private:
   const uint32_t _gainId, _panId, _muteId;
   float _gainValue, _panValue, _panAmountL, _panAmountR, _muteValue;

   ImRt::Buffer _bufferOscilloscope, _bufferVolume;
   uint32_t _posOscilloscope { 0 }, _posVolume { 0 };
   uint32_t _numFramesOscilloscope { 4096 }, _numFramesVolume { 1024 };
};
