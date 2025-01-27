#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include <cstring> // choc_...FIFO.h needs it

#include <containers/choc_VariableSizeFIFO.h>

/* ------------------------------------------------------ */
/*                    parameter layout                    */
/* ------------------------------------------------------ */

class ParameterLayout {
public:
   ParameterLayout(
      uint32_t id, std::string name, float min, float max, float init);
   ParameterLayout() = delete;

   uint32_t id();
   const char* name();
   float min();
   float max();
   float init();

protected:
   const uint32_t _id;
   const std::string _name;
   const float _min, _max, _init;
};

/* ------------------------------------------------------ */
/*                      gui parameter                     */
/* ------------------------------------------------------ */

struct GuiParameter : public ParameterLayout {
   GuiParameter(
      uint32_t id, std::string name, float min, float max, float init);
   GuiParameter() = delete;

   float value;
};

/* ------------------------------------------------------ */
/*                      dsp parameter                     */
/* ------------------------------------------------------ */

class DspParameter : public ParameterLayout {
   friend class DspParameters;

public:
   DspParameter(
      uint32_t id, std::string name, float min, float max, float init);
   DspParameter() = delete;

   void push(float& newValue);
   void update();

private:
   float value;
   choc::fifo::VariableSizeFIFO _fifo;
};

/* ------------------------------------------------------ */
/*                     gui parameters                     */
/* ------------------------------------------------------ */

class DspParameters;

class GuiParameters {
public:
   GuiParameters(const DspParameters& audioParameters);
   GuiParameters() = delete;

   GuiParameter* byId(uint32_t paramId);

private:
   std::map<uint32_t, std::unique_ptr<GuiParameter>> _params;
};

/* ------------------------------------------------------ */
/*                     dsp parameters                     */
/* ------------------------------------------------------ */

class DspParameters {
   friend class GuiParameters;

public:
   void add(
      uint32_t paramId, std::string name, float min, float max, float init);

   void push(uint32_t paramId, float& newValue);
   void update(uint32_t paramId);

   float value(uint32_t paramId);

private:
   std::map<uint32_t, std::unique_ptr<DspParameter>> _params;
};
