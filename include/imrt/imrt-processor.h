#pragma once

#include <RtAudio.h>

namespace ImRt {

template <typename Derived> class Processor {
public:
   struct Config {
      int numChannels         = 2;
      unsigned int sampleRate = 44100;
      unsigned int bufferSize = 0; // 0 means as small as possible
   };

   Processor(Config config)
      : _config(config)
   {
      if (_audio.getDeviceIds().size() == 0)
         abort();

      _parameters.deviceId     = _audio.getDefaultInputDevice();
      _parameters.nChannels    = _config.numChannels;
      _parameters.firstChannel = 0;
   }

   virtual ~Processor()
   {
      if (_audio.isStreamRunning())
         _audio.stopStream();
      if (_audio.isStreamOpen())
         _audio.closeStream();
   }

   void run()
   {
      if (_audio.openStream(nullptr, &_parameters, RTAUDIO_FLOAT32,
             _config.sampleRate, &_config.bufferSize, &Static_audioCallback,
             this))
         abort();

      if (_audio.startStream())
         abort();
   }

   unsigned int sampleRate()
   {
      return _audio.getStreamSampleRate();
   }

   unsigned int numChannels()
   {
      return _config.numChannels;
   }

private:
   RtAudio _audio;
   RtAudio::StreamParameters _parameters;
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