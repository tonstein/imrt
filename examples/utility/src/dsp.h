#pragma once

#include <imrt.h>

class Dsp : public ImRt::Dsp<Dsp>
{
public:
   Dsp(ImRt::DspSettings settings = ImRt::DspSettings());

   int process(ImRt::Buffer& in, ImRt::Buffer& out, uint32_t numFrames);

private:
   float gainValue, panValue, panAmountL, panAmountR;
};
