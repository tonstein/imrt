#include "imrt-parameters.h"

Parameter::Parameter(std::string name, float min, float max, float init)
   : name(name)
   , min(min)
   , max(max)
   , init(init)
   , value(init)
{
   fifo.reset(4 * sizeof(float));
}

Parameter::~Parameter() { }

void Parameter::push(float& newValue)
{
   fifo.push(&newValue, sizeof(float));
}

void Parameter::update()
{
   fifo.pop([this](const void* data, uint32_t size) {
      if (size == sizeof(float))
         value = *(float*)data;
   });
}

Parameters::Parameters() { }
Parameters::~Parameters() { }

uint32_t Parameters::add(std::string name, float min, float max, float init)
{
   uint32_t id = nextId;
   auto p      = std::make_unique<Parameter>(name, min, max, init);
   parameters.insert_or_assign(id, std::move(p));
   ++nextId;
   return id;
}

void Parameters::push(uint32_t paramId, float& newValue)
{
   parameters.at(paramId)->push(newValue);
}

void Parameters::update(uint32_t paramId)
{
   parameters.at(paramId)->update();
}

std::string Parameters::name(uint32_t paramId)
{
   return parameters.at(paramId)->name;
}

float Parameters::min(uint32_t paramId)
{
   return parameters.at(paramId)->min;
}

float Parameters::max(uint32_t paramId)
{
   return parameters.at(paramId)->max;
}

float Parameters::init(uint32_t paramId)
{
   return parameters.at(paramId)->init;
}

float Parameters::value(uint32_t paramId)
{
   return parameters.at(paramId)->value;
}
