#pragma once

#include <RtAudio.h>
#include "imrt-params.h"

#include "imrt-constants.h"

namespace ImRt {

/* -------------------------------------------------------------------------- */
/*                     AUDIO SETTINGS                                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief Settings passed to the digital signal processor (Dsp) class
 * constructor to specify the number of input and output channels, the sample
 * rate and the buffer size of the stream that the processor opens when its
 * Dsp::run() method is called.
 */
struct DspSettings
{
   int numChannelsIn   = 2;
   int numChannelsOut  = 2;
   uint32_t sampleRate = 44100;
   uint32_t bufferSize = 0; // 0 means as small as possible
};

/* -------------------------------------------------------------------------- */
/*                        DSP CLASS                                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief Digital signal processor (DSP) template class to inherit from.
 *
 * @tparam Use the inheritor class as template parameter.
 */
template <typename Derived>
class Dsp
{
   template <typename, typename>
   friend class Gui;

public:
   /**
    * @brief Construct a new digital signal processor (DSP) object with the
    * given settings.
    *
    * @param settings If not specified the default settings are:
    * - Number of input channels: 2
    * - Number of output channels: 2
    * - Sample rate: 41000
    * - Buffer size: as small as possible
    */
   Dsp(DspSettings settings = DspSettings())
      : _settings(settings)
   {
      auto defaultIn  = _dac.getDefaultInputDevice();
      auto defaultOut = _dac.getDefaultOutputDevice();

      if ((defaultIn == 0) || (defaultOut == 0))
      {
         abort();
      }

      _paramsIn.deviceId     = defaultIn;
      _paramsIn.nChannels    = _settings.numChannelsIn;
      _paramsIn.firstChannel = 0;

      _paramsOut.deviceId     = defaultOut;
      _paramsOut.nChannels    = _settings.numChannelsIn;
      _paramsOut.firstChannel = 0;
   }

   /**
    * @brief Stops the stream opened by the digital signal processor (DSP)
    * object and destroys the object.
    */
   virtual ~Dsp()
   {
      if (_dac.isStreamRunning())
      {
         _dac.stopStream();
      }
      if (_dac.isStreamOpen())
      {
         _dac.closeStream();
      }
   }

   /**
    * @brief Opens an input and an output stream with the settings passed to the
    * DSP constructor and passes the input buffer to and receives the output
    * buffer from the Dsp::process() callback method.
    */
   void run()
   {
      auto options  = RtAudio::StreamOptions();
      options.flags = 0;
      // options.flags |= RTAUDIO_NONINTERLEAVED;
      options.flags |= RTAUDIO_MINIMIZE_LATENCY;
      options.flags |= RTAUDIO_SCHEDULE_REALTIME;
      // options.flags |= RTAUDIO_ALSA_USE_DEFAULT;
      options.streamName = "imrt-stream";
      options.priority   = 10; // 99

      if (_dac.openStream(
             &_paramsOut, &_paramsIn, RTAUDIO_FLOAT32, _settings.sampleRate,
             &_settings.bufferSize, &AudioCallback, this, &options
          ))
      {
         abort();
      }

      if (_dac.startStream())
      {
         abort();
      }
   }

   /**
    * @brief Returns the actual sample rate of the (open) stream, which may
    * differ slightly from the specified one. one. If a stream is not open, a
    * value of zero is returned.
    */
   uint32_t sampleRate()
   {
      return _dac.getStreamSampleRate();
   }

   /**
    * @brief Creates a new DspParameter with the given ParameterLayout and
    * adds this parameter to the DspParameters collection managed by the Dsp
    * object.
    *
    * @param layout The skeleton for the new DSP parameter consisting of an
    * ID, a name, a maximum, minimum and initial value.
    */
   void addParameter(ParameterLayout& layout)
   {
      parameters.addParameter(layout);
   }

   /**
    * @brief Announces a change of a parameter value by pushing the new value to
    * a multiple writer, single consumer parameter FIFO. This parameter change
    * should be read in the Dsp::process() method of the digital signal
    * processor by calling the Dsp::updatedParameterValue() and the
    * Dsp::parameterValue() methods. Multiple write threads may have to briefly
    * spin-wait for each other, but the reader thread is not blocked by the
    * activity of writers.
    *
    * @param paramId The ID of the parameter whose value should change.
    * @param newValue The value to which the parameter value should change.
    */
   void announceParameterChange(uint32_t paramId, float& newValue)
   {
      parameters.announceChange(paramId, newValue);
   }

   /**
    * @brief Updates a possible DspParameter value change by consuming the
    * value from the FIFO to which Dsp::announceParameterChange()
    * pushes. The new value of the DspParameter can be obtained by calling the
    * Dsp::parameterValue() method.
    *
    * @param paramId The ID of the parameter whose value could have changed.
    */
   float updatedParameterValue(uint32_t paramId)
   {
      return parameters.updatedValue(paramId);
   }

   /**
    * @brief Returns the value of a DspParameter.
    *
    * @param paramId The ID of the DspParameter.
    */
   float parameterValue(uint32_t paramId)
   {
      return parameters.value(paramId);
   }

private:
   /**
    * @brief The audio callback method that is fed with the input and output
    * stream of the digital signal processor. This method must be implemented
    * by the inheritor class of the DSP template class. Within this method
    * one can call the Dsp::updatedParameterValue() method of the member
    * DspParameters parameters to the announced value (cf.
    * announceParameterChange()).
    *
    * @param in Input buffer.
    * @param out Output buffer.
    * @param numFrames Number of frames of the input and output buffer
    * respectively (buffer size).
    * @return int To continue normal stream operation, return 0.
    * To stop the stream and drain the output buffer, return 1.
    * To abort the stream immediately, the client should return 2.
    */
   int process(Buffer& in, Buffer& out, uint32_t numFrames)
   {
      return static_cast<Derived*>(this)->process(in, out, numFrames);
   }

   /* ----------------------------------------------------------------------- */
   /*                    INTERNAL DETAILS                                     */
   /* ----------------------------------------------------------------------- */

private:
   RtAudio _dac;
   RtAudio::StreamParameters _paramsIn, _paramsOut;
   DspSettings _settings;
   ImRt::Buffer _in, _out;
   DspParameters parameters;

private:
   int
   audioCallback(void* outputBuffer, void* inputBuffer, uint32_t nBufferFrames)
   {
      uint32_t n = _settings.numChannelsIn;
      _in.resize({ n, nBufferFrames });

      for (uint32_t frame = 0; frame < nBufferFrames; ++frame)
      {
         for (uint32_t channel = 0; channel < n; ++channel)
         {
            _in.getSample(channel, frame)
               = ((float*)inputBuffer)[n * frame + channel];
         }
      }

      uint32_t m = _settings.numChannelsOut;
      _out.resize({ m, nBufferFrames });

      int r = process(_in, _out, nBufferFrames);

      for (uint32_t frame = 0; frame < nBufferFrames; ++frame)
      {
         for (uint32_t channel = 0; channel < m; ++channel)
         {
            ((float*)outputBuffer)[m * frame + channel]
               = _out.getSample(channel, frame);
         }
      }

      return r;
   }

   static int AudioCallback(
      void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
      double streamTime, unsigned int status, void* userData
   )
   {
      return static_cast<Dsp*>(userData)->audioCallback(
         outputBuffer, inputBuffer, nBufferFrames
      );
   }
};

} // namespace ImRt
