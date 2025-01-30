#include "dsp.h"

Dsp::Dsp(ImRt::DspSettings settings)
   : ImRt::Dsp<Dsp>(settings)
{
}

int Dsp::process(ImRt::Buffer& in, ImRt::Buffer& out, uint32_t numFrames)
{
   view = in.getView();
   out  = in;
   return 0;
}
