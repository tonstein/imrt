#pragma once

#include <imrt.h>

class Dsp : public ImRt::Dsp<Dsp>
{
public:
   Dsp(ImRt::DspSettings settings = ImRt::DspSettings());

   int process(ImRt::Buffer& out, ImRt::Buffer& in, uint32_t numFrames);

private:
   float gainValue, panValue, panAmountL, panAmountR;
};
