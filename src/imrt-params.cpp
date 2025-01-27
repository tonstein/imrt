#include "imrt-params.h"
#include <cstdint>
#include <memory>

/* ------------------------------------------------------ */
/*                    parameter layout                    */
/* ------------------------------------------------------ */

ParameterLayout::ParameterLayout(
   std::string name, float min, float max, float init)
   : _name(name)
   , _min(min)
   , _max(max)
   , _init(init)
{
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

GuiParameter::GuiParameter(std::string name, float min, float max, float init)
   : ParameterLayout(name, min, max, init)
   , value(init)
{
}

/* ------------------------------------------------------ */
/*                     gui parameters                     */
/* ------------------------------------------------------ */

GuiParameters::GuiParameters(const DspParameters& audioParameters)
{
   for (auto i = audioParameters._params.begin();
        i != audioParameters._params.end(); i++) {
      uint32_t id = i->first;
      _ids.push_back(i->first);

      auto p         = i->second.get();
      auto parameter = std::make_unique<GuiParameter>(
         p->name(), p->min(), p->max(), p->init());
      _params.insert_or_assign(i->first, std::move(parameter));
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

DspParameter::DspParameter(std::string name, float min, float max, float init)
   : ParameterLayout(name, min, max, init)
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
   uint32_t paramId, std::string name, float min, float max, float init)
{
   // [FixMe] Check whether paramId already exists.
   auto parameter = std::make_unique<DspParameter>(name, min, max, init);
   _params.insert_or_assign(paramId, std::move(parameter));
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
