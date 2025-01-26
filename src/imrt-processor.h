#pragma once

#include <RtAudio.h>

#include <audio/choc_SampleBuffers.h>

namespace ImRt {

using Buffer
   = choc::buffer::AllocatedBuffer<float, choc::buffer::SeparateChannelLayout>;

struct Stream {
   int numChannelsIn       = 2;
   int numChannelsOut      = 2;
   unsigned int sampleRate = 44100;
   unsigned int bufferSize = 0; // 0 means as small as possible
};

template <typename Derived> class Processor {
public:
   struct Config {
      Stream stream;
   };

   Processor(Config config)
      : _config(config)
   {
      auto defaultIn  = _dac.getDefaultInputDevice();
      auto defaultOut = _dac.getDefaultOutputDevice();

      if ((defaultIn == 0) || (defaultOut == 0))
         abort();

      _paramsIn.deviceId = defaultIn;
      _paramsIn.nChannels
         = _config.stream
              .numChannelsIn; // [FixMe] Check number of available channels.
      _paramsIn.firstChannel = 0;

      _paramsOut.deviceId = defaultOut;
      _paramsOut.nChannels
         = _config.stream
              .numChannelsIn; // [FixMe] Check number of available channels.
      _paramsOut.firstChannel = 0;
   }

   virtual ~Processor()
   {
      if (_dac.isStreamRunning())
         _dac.stopStream();
      if (_dac.isStreamOpen())
         _dac.closeStream();
   }

   void run()
   {
      if (_dac.openStream(&_paramsOut, &_paramsIn, RTAUDIO_FLOAT32,
             _config.stream.sampleRate, &_config.stream.bufferSize,
             &AudioCallback, this))
         abort();

      if (_dac.startStream())
         abort();
   }

   unsigned int sampleRate()
   {
      return _dac.getStreamSampleRate();
   }

private:
   RtAudio _dac;
   RtAudio::StreamParameters _paramsIn, _paramsOut;
   Config _config;

private:
   int process(Buffer& out, Buffer& in)
   {
      return static_cast<Derived*>(this)->process(out, in);
   }

private:
   int audioCallback(void* outputBuffer, void* inputBuffer,
      unsigned int nBufferFrames, double streamTime, unsigned int status)
   {

      unsigned int n = _config.stream.numChannelsIn;
      ImRt::Buffer in;
      in.resize({ n, nBufferFrames });

      for (unsigned int frame = 0; frame < nBufferFrames; ++frame) {
         for (unsigned int channel = 0; channel < n; ++channel) {
            in.getSample(channel, frame)
               = ((float*)inputBuffer)[n * frame + channel];
         }
      }

      unsigned int m = _config.stream.numChannelsOut;
      ImRt::Buffer out;
      out.resize({ m, nBufferFrames });

      int r = process(out, in);

      for (unsigned int frame = 0; frame < nBufferFrames; ++frame) {
         for (unsigned int channel = 0; channel < m; ++channel) {
            ((float*)outputBuffer)[m * frame + channel]
               = out.getSample(channel, frame);
         }
      }

      return r;
   }

   static int AudioCallback(void* outputBuffer, void* inputBuffer,
      unsigned int nBufferFrames, double streamTime, unsigned int status,
      void* userData)
   {
      return static_cast<Processor*>(userData)->audioCallback(
         outputBuffer, inputBuffer, nBufferFrames, streamTime, status);
   }
};

} // namespace ImRt