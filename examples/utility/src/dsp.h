#pragma once

#include <cstdint>
#include <imrt.h>

class Dsp : public ImRt::Dsp<Dsp>
{
public:
   Dsp(ImRt::DspSettings settings = ImRt::DspSettings());

   int process(ImRt::Buffer& in, ImRt::Buffer& out, uint32_t numFrames);

   ImRt::BufferView oscView, volView;

private:
   const uint32_t _gainId, _panId, _muteId;

   ImRt::Buffer _oscBuffer, _volBuffer;
   uint32_t _oscPos { 0 }, _volPos { 0 };
   uint32_t _oscNumFrames { 4096 }, _volNumFrames { 1024 };
};
