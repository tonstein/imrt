#pragma once

#include <RtAudio.h>

#include <audio/choc_SampleBuffers.h>

#include "imrt-params.h"

namespace ImRt {

using Buffer
   = choc::buffer::AllocatedBuffer<float, choc::buffer::SeparateChannelLayout>;

/* ------------------------------------------------------ */
/*                      audio settings                    */
/* ------------------------------------------------------ */

struct DspSettings {
   int numChannelsIn   = 2;
   int numChannelsOut  = 2;
   uint32_t sampleRate = 44100;
   uint32_t bufferSize = 0; // 0 means as small as possible
};

/* ------------------------------------------------------ */
/*                           dsp                          */
/* ------------------------------------------------------ */

template <typename Derived> class Dsp {
   template <typename, typename> friend class Gui;

public:
   Dsp(DspSettings settings = DspSettings())
      : _settings(settings)
   {
      auto defaultIn  = _dac.getDefaultInputDevice();
      auto defaultOut = _dac.getDefaultOutputDevice();

      if ((defaultIn == 0) || (defaultOut == 0))
         abort();

      _paramsIn.deviceId     = defaultIn;
      _paramsIn.nChannels    = _settings.numChannelsIn;
      _paramsIn.firstChannel = 0;

      _paramsOut.deviceId     = defaultOut;
      _paramsOut.nChannels    = _settings.numChannelsIn;
      _paramsOut.firstChannel = 0;
   }

   virtual ~Dsp()
   {
      if (_dac.isStreamRunning())
         _dac.stopStream();
      if (_dac.isStreamOpen())
         _dac.closeStream();
   }

   void run()
   {
      if (_dac.openStream(&_paramsOut, &_paramsIn, RTAUDIO_FLOAT32,
             _settings.sampleRate, &_settings.bufferSize, &AudioCallback, this))
         abort();

      if (_dac.startStream())
         abort();
   }

   uint32_t sampleRate()
   {
      return _dac.getStreamSampleRate();
   }

   void pushParameter(uint32_t paramId, float& newValue)
   {
      parameters.push(paramId, newValue);
   }

private:
   int process(Buffer& in, Buffer& out, uint32_t numFrames)
   {
      return static_cast<Derived*>(this)->process(in, out, numFrames);
   }

protected:
   AudioParameters parameters;

private:
   RtAudio _dac;
   RtAudio::StreamParameters _paramsIn, _paramsOut;
   DspSettings _settings;
   ImRt::Buffer in, out;

   /* ------------------------------------------------------ */
   /*                     audio callback                     */
   /* ------------------------------------------------------ */

private:
   int audioCallback(
      void* outputBuffer, void* inputBuffer, uint32_t nBufferFrames)
   {
      uint32_t n = _settings.numChannelsIn;
      in.resize({ n, nBufferFrames });

      for (uint32_t frame = 0; frame < nBufferFrames; ++frame) {
         for (uint32_t channel = 0; channel < n; ++channel) {
            in.getSample(channel, frame)
               = ((float*)inputBuffer)[n * frame + channel];
         }
      }

      uint32_t m = _settings.numChannelsOut;
      out.resize({ m, nBufferFrames });

      int r = process(in, out, nBufferFrames);

      for (uint32_t frame = 0; frame < nBufferFrames; ++frame) {
         for (uint32_t channel = 0; channel < m; ++channel) {
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
      return static_cast<Dsp*>(userData)->audioCallback(
         outputBuffer, inputBuffer, nBufferFrames);
   }
};

} // namespace ImRt