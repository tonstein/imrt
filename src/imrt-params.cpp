#include "imrt-params.h"
#include <cstdint>
#include <memory>

/* ------------------------------------------------------ */
/*                    parameter layout                    */
/* ------------------------------------------------------ */

ParameterLayout::ParameterLayout(
   uint32_t id, std::string name, float min, float max, float init)
   : _id(id)
   , _name(name)
   , _min(min)
   , _max(max)
   , _init(init)
{
}

uint32_t ParameterLayout::id()
{
   return _id;
}

const char* ParameterLayout::name()
{
   return _name.c_str();
}

float ParameterLayout::min()
{
   return _min;
}

float ParameterLayout::max()
{
   return _max;
}

float ParameterLayout::init()
{
   return _init;
}

/* ------------------------------------------------------ */
/*                      parameter IDs                     */
/* ------------------------------------------------------ */

std::vector<uint32_t> ParameterIds::ids()
{
   return _ids;
}

/* ------------------------------------------------------ */
/*                      gui parameter                     */
/* ------------------------------------------------------ */

GuiParameter::GuiParameter(
   uint32_t id, std::string name, float min, float max, float init)
   : ParameterLayout(id, name, min, max, init)
   , value(init)
{
}

/* ------------------------------------------------------ */
/*                     gui parameters                     */
/* ------------------------------------------------------ */

GuiParameters::GuiParameters(const DspParameters& audioParameters)
{
   for (auto iterator = audioParameters._params.begin();
        iterator != audioParameters._params.end(); iterator++) {
      uint32_t id = iterator->first;
      _ids.push_back(iterator->first);

      auto dspParam = iterator->second.get();
      auto guiParam = std::make_unique<GuiParameter>(dspParam->id(),
         dspParam->name(), dspParam->min(), dspParam->max(), dspParam->init());
      _params.insert_or_assign(iterator->first, std::move(guiParam));
   }
}

GuiParameter* GuiParameters::byId(uint32_t paramId)
{
   // [FixMe] Check if paramId is available.
   return _params.at(paramId).get();
}

/* ------------------------------------------------------ */
/*                     audio parameter                    */
/* ------------------------------------------------------ */

DspParameter::DspParameter(
   uint32_t id, std::string name, float min, float max, float init)
   : ParameterLayout(id, name, min, max, init)
   , value(init)
{
   _fifo.reset(4 * sizeof(float));
}

void DspParameter::push(float& newValue)
{
   _fifo.push(&newValue, sizeof(float));
}

void DspParameter::update()
{
   _fifo.pop([this](const void* data, uint32_t size) {
      if (size == sizeof(float))
         value = *(float*)data;
   });
}

/* ------------------------------------------------------ */
/*                    audio parameters                    */
/* ------------------------------------------------------ */

void DspParameters::add(
   uint32_t id, std::string name, float min, float max, float init)
{
   // [FixMe] Check whether paramId already exists.
   auto parameter = std::make_unique<DspParameter>(id, name, min, max, init);
   _params.insert_or_assign(id, std::move(parameter));
}

void DspParameters::push(uint32_t paramId, float& newValue)
{
   // [FixMe] Check if paramId is available.
   _params.at(paramId)->push(newValue);
}

void DspParameters::update(uint32_t paramId)
{
   // [FixMe] Check if paramId is available.
   _params.at(paramId)->update();
}

float DspParameters::value(uint32_t paramId)
{
   // [FixMe] Check if paramId is available.
   return _params.at(paramId)->value;
}
