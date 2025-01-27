#include "imrt-params.h"
#include <cstdint>
#include <memory>

namespace ImRt {

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
/*                      gui parameter                     */
/* ------------------------------------------------------ */

GuiParameter::GuiParameter(
   uint32_t id, std::string name, float min, float max, float init)
   : ParameterLayout(id, name, min, max, init)
   , value(init)
{
}

/* ------------------------------------------------------ */
/*                      dsp parameter                     */
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
/*                     gui parameters                     */
/* ------------------------------------------------------ */

GuiParameters::GuiParameters(const DspParameters& audioParameters)
{
   for (auto iterator = audioParameters._params.begin();
        iterator != audioParameters._params.end(); iterator++) {
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
/*                     dsp parameters                     */
/* ------------------------------------------------------ */

void DspParameters::add(ParameterLayout& layout)
{
   // [FixMe] Check whether paramId already exists.
   auto parameter = std::make_unique<DspParameter>(
      layout.id(), layout.name(), layout.min(), layout.max(), layout.init());
   _params.insert_or_assign(layout.id(), std::move(parameter));
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

} // namespace ImRt