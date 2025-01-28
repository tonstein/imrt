#include "imrt-params.h"
#include <cstdint>
#include <memory>

namespace ImRt {

/* ------------------------------------------------------ */
/*                    parameter layout                    */
/* ------------------------------------------------------ */

ParameterLayout::ParameterLayout(
   uint32_t id, std::string name, float min, float max, float init
)
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
/*                      dsp parameter                     */
/* ------------------------------------------------------ */

DspParameter::DspParameter(ParameterLayout layout)
   : ParameterLayout(
        layout.id(), layout.name(), layout.min(), layout.max(), layout.init()
     )
   , value(layout.init())
{
   _fifo.reset(120 * sizeof(float));
}

void DspParameter::announce(float& newValue)
{
   _fifo.push(&newValue, sizeof(float));
}

void DspParameter::update()
{
   _fifo.pop(
      [this](const void* data, uint32_t size)
      {
         if (size == sizeof(float))
         {
            value = *(float*)data;
         }
      }
   );
}

/* ------------------------------------------------------ */
/*                      gui parameter                     */
/* ------------------------------------------------------ */

GuiParameter::GuiParameter(ParameterLayout layout)
   : ParameterLayout(
        layout.id(), layout.name(), layout.min(), layout.max(), layout.init()
     )
   , value(layout.init())
{
}

/* ------------------------------------------------------ */
/*                     dsp parameters                     */
/* ------------------------------------------------------ */

void DspParameters::add(ParameterLayout& layout)
{
   // [FixMe] Check whether paramId already exists.
   auto parameter = std::make_unique<DspParameter>(layout);
   _params.insert_or_assign(layout.id(), std::move(parameter));
}

void DspParameters::announce(uint32_t paramId, float& newValue)
{
   // [FixMe] Check if paramId is available.
   _params.at(paramId)->announce(newValue);
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

/* ------------------------------------------------------ */
/*                     gui parameters                     */
/* ------------------------------------------------------ */

GuiParameters::GuiParameters(const DspParameters& audioParameters)
{
   for (auto iterator = audioParameters._params.begin();
        iterator != audioParameters._params.end(); iterator++)
   {
      auto dspParam = iterator->second.get();
      auto guiParam = std::make_unique<GuiParameter>(ParameterLayout(
         dspParam->id(), dspParam->name(), dspParam->min(), dspParam->max(),
         dspParam->init()
      ));
      _params.insert_or_assign(iterator->first, std::move(guiParam));
   }
}

GuiParameter* GuiParameters::byId(uint32_t paramId)
{
   // [FixMe] Check if paramId is available.
   return _params.at(paramId).get();
}

} // namespace ImRt
