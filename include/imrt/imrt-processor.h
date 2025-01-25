#pragma once

#include <RtAudio.h>

namespace ImRt {

template <typename Derived> class Processor {
public:
   struct Config {
      int numChannelsIn       = 2;
      int numChannelsOut      = 2;
      unsigned int sampleRate = 44100;
      unsigned int bufferSize = 0; // 0 means as small as possible
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
         = _config.numChannelsIn; // [FixMe] Check number of available channels.
      _paramsIn.firstChannel = 0;

      _paramsOut.deviceId = defaultOut;
      _paramsOut.nChannels
         = _config.numChannelsIn; // [FixMe] Check number of available channels.
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
             _config.sampleRate, &_config.bufferSize, &Static_audioCallback,
             this))
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
   int process(float* out, float* in, unsigned int bufferSize)
   {
      return static_cast<Derived*>(this)->process(out, in, bufferSize);
   }

private:
   int audioCallback(void* outputBuffer, void* inputBuffer,
      unsigned int nBufferFrames, double streamTime, unsigned int status)
   {
      float* in  = (float*)inputBuffer;
      float* out = (float*)outputBuffer;
      return process(out, in, nBufferFrames);
   }

   static int Static_audioCallback(void* outputBuffer, void* inputBuffer,
      unsigned int nBufferFrames, double streamTime, unsigned int status,
      void* userData)
   {
      return static_cast<Processor*>(userData)->audioCallback(
         outputBuffer, inputBuffer, nBufferFrames, streamTime, status);
   }
};

} // namespace ImRt