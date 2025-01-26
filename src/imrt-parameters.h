#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include <cstring> // choc_...FIFO.h needs it

#include <containers/choc_VariableSizeFIFO.h>

class Parameter {
   friend class Parameters;

public:
   Parameter(std::string name, float min, float max, float init);
   Parameter() = delete;
   ~Parameter();

   void push(float newValue);
   void update();

private:
   std::string name;
   float min, max, init, value;
   choc::fifo::VariableSizeFIFO fifo;
};

class Parameters {
public:
   Parameters();
   ~Parameters();

   uint32_t add(std::string name, float min, float max, float init);

   void push(uint32_t paramId, float& newValue);
   void update(uint32_t paramId);

   std::string name(uint32_t paramId);
   float min(uint32_t paramId);
   float max(uint32_t paramId);
   float init(uint32_t paramId);
   float value(uint32_t paramId);

private:
   uint32_t nextId = 0;
   std::map<uint32_t, std::unique_ptr<Parameter>> parameters;
};