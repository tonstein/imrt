#pragma once

#include <RtAudio.h>

namespace ImRt {

template <typename Derived> class Processor {
public:
   struct Config {
      int numChannels         = 2;
      unsigned int sampleRate = 44100;
   };

   Processor(Config config)
      : _numChannels(config.numChannels)
      , _sampleRate(config.sampleRate)
   {
      if (_audio.getDeviceIds().size() == 0)
         abort();

      _parameters.deviceId     = _audio.getDefaultInputDevice();
      _parameters.nChannels    = _numChannels;
      _parameters.firstChannel = 0;
   }

   ~Processor()
   {
      if (_audio.isStreamRunning())
         _audio.stopStream();
      if (_audio.isStreamOpen())
         _audio.closeStream();
   }

   void run()
   {
      unsigned int frames = 0; // 0 means as small as possible
      if (_audio.openStream(nullptr, &_parameters, RTAUDIO_FLOAT32, _sampleRate,
             &frames, &Static_audioCallback, this))
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
      return _numChannels;
   }

private:
   RtAudio _audio;
   RtAudio::StreamParameters _parameters;
   unsigned int _sampleRate, _numChannels;

private:
   int audioCallback(void* outputBuffer, void* inputBuffer,
      unsigned int nBufferFrames, double streamTime, unsigned int status)
   {
      return static_cast<Derived*>(this)->audioCallback(
         outputBuffer, inputBuffer, nBufferFrames, streamTime, status);
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