#include "imrt-parameters.h"

/* ------------------------------------------------------ */
/*                        Parameter                       */
/* ------------------------------------------------------ */

Parameter::Parameter(std::string name, float min, float max, float init)
   : _name(name)
   , _min(min)
   , _max(max)
   , _init(init)
   , value(init)
{
}

Parameter::~Parameter() { }

const char* Parameter::name()
{
   return _name.c_str();
}

float Parameter::min()
{
   return _min;
}

float Parameter::max()
{
   return _max;
}

float Parameter::init()
{
   return _init;
}

/* ------------------------------------------------------ */
/*                       Parameters                       */
/* ------------------------------------------------------ */

Parameters::Parameters(const AudioParameters& audioParameters)
{
   for (auto i = audioParameters._params.begin();
        i != audioParameters._params.end(); i++) {
      _ids.push_back(i->first);
      _params.insert_or_assign(i->first, i->second.get());
   }
}

Parameters::~Parameters() { }

std::vector<uint32_t> Parameters::ids()
{
   return _ids;
}

Parameter* Parameters::byId(uint32_t paramId)
{
   return _params.at(paramId);
}

/* ------------------------------------------------------ */
/*                     AudioParameter                     */
/* ------------------------------------------------------ */

AudioParameter::AudioParameter(
   std::string name, float min, float max, float init)
   : Parameter(name, min, max, init)
{
   _fifo.reset(4 * sizeof(float));
}

AudioParameter::~AudioParameter() { }

void AudioParameter::push(float& newValue)
{
   _fifo.push(&newValue, sizeof(float));
}

void AudioParameter::update()
{
   _fifo.pop([this](const void* data, uint32_t size) {
      if (size == sizeof(float))
         value = *(float*)data;
   });
}

/* ------------------------------------------------------ */
/*                     AudioParameters                    */
/* ------------------------------------------------------ */

AudioParameters::AudioParameters() { }
AudioParameters::~AudioParameters() { }

uint32_t AudioParameters::add(
   std::string name, float min, float max, float init)
{
   uint32_t id = _nextId;
   auto p      = std::make_unique<AudioParameter>(name, min, max, init);
   _params.insert_or_assign(id, std::move(p));
   ++_nextId;
   return id;
}

std::vector<uint32_t> AudioParameters::ids()
{
   return _ids;
}

void AudioParameters::push(uint32_t paramId, float& newValue)
{
   _params.at(paramId)->push(newValue);
}

void AudioParameters::update(uint32_t paramId)
{
   _params.at(paramId)->update();
}

float AudioParameters::value(uint32_t paramId)
{
   return _params.at(paramId)->value;
}
