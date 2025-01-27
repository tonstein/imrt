#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <cstring> // choc_...FIFO.h needs it

#include <containers/choc_VariableSizeFIFO.h>

/* ------------------------------------------------------ */
/*                    parameter layout                    */
/* ------------------------------------------------------ */

class ParameterLayout {
public:
   ParameterLayout(std::string name, float min, float max, float init);
   ParameterLayout() = delete;

   const char* name();
   float min();
   float max();
   float init();

protected:
   std::string _name;
   float _min, _max, _init;
};

/* ------------------------------------------------------ */
/*                      parameter IDs                     */
/* ------------------------------------------------------ */

class ParameterIds {
public:
   std::vector<uint32_t> ids();

protected:
   std::vector<uint32_t> _ids;
};

/* ------------------------------------------------------ */
/*                      gui parameter                     */
/* ------------------------------------------------------ */

struct GuiParameter : public ParameterLayout {
   GuiParameter(std::string name, float min, float max, float init);
   GuiParameter() = delete;

   float value;
};

/* ------------------------------------------------------ */
/*                     gui parameters                     */
/* ------------------------------------------------------ */

class DspParameters;

class GuiParameters : ParameterIds {
public:
   GuiParameters(const DspParameters& audioParameters);
   GuiParameters() = delete;

   GuiParameter* byId(uint32_t paramId);

private:
   std::map<uint32_t, std::unique_ptr<GuiParameter>> _params;
};

/* ------------------------------------------------------ */
/*                      dsp parameter                     */
/* ------------------------------------------------------ */

class DspParameter : public ParameterLayout {
   friend class DspParameters;

public:
   DspParameter(std::string name, float min, float max, float init);
   DspParameter() = delete;

   void push(float& newValue);
   void update();

private:
   float value;
   choc::fifo::VariableSizeFIFO _fifo;
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
