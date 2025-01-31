#pragma once

#include "audio/choc_SampleBuffers.h"

namespace ImRt {

using Buffer
   = choc::buffer::AllocatedBuffer<float, choc::buffer::SeparateChannelLayout>;
using BufferView
   = choc::buffer::BufferView<float, choc::buffer::SeparateChannelLayout>;

} // namespace ImRt
