#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include <cstring> // choc_...FIFO.h needs it

#include <containers/choc_VariableSizeFIFO.h>

namespace ImRt {

/* -------------------------------------------------------------------------- */
/*                    PARAMETER LAYOUT                                        */
/* -------------------------------------------------------------------------- */

/**
 * @brief This class serves as a skeleton for defining a parameter. Note that a
 * parameter layout does not have a current parameter value, but the derived
 * classes GuiParameter and DspParameter have. These derived classes deal with
 * the parameter value in different ways (cf. GuiParameter and DspParameter).
 */
class ParameterLayout
{
public:
   /**
    * @brief Construct a new parameter layout object that can be added to a
    * collection of DSP parameters (cf. class DspParameters).
    *
    * @param id A unique ID used to identify the parameter e.g. when updating
    * the GUI resp. DSP value of the parameter or when creating a GUI widget for
    * the parameter.
    * @param name The name displayed in the GUI widgets of the parameter.
    * @param min The minimum value that the parameter can have.
    * @param max The maximum value that the parameter can have.
    * @param init The initial resp. default value for the parameter.
    */
   ParameterLayout(
      uint32_t id, std::string name, float min, float max, float init
   );
   ParameterLayout() = delete;

   /**
    * @brief Returns the unique ID of the parameter.
    */
   uint32_t id();

   /**
    * @brief Returns the name of the parameter.
    */
   const char* name();

   /**
    * @brief Returns the minimum value of the parameter.
    */
   float min();

   /**
    * @brief Returns the maximum value of the parameter.
    */
   float max();

   /**
    * @brief Returns the initial resp. default value of the parameter.
    */
   float init();

protected:
   const uint32_t _id;
   const std::string _name;
   const float _min, _max, _init;
};

/* -------------------------------------------------------------------------- */
/*                      DSP PARAMETER                                         */
/* -------------------------------------------------------------------------- */

class DspParameter : public ParameterLayout
{
   friend class DspParameters;

public:
   DspParameter(ParameterLayout layout);
   DspParameter() = delete;

   void push(float& newValue);
   void update();

private:
   float value;
   choc::fifo::VariableSizeFIFO _fifo;
};

/* -------------------------------------------------------------------------- */
/*                      GUI PARAMETER                                         */
/* -------------------------------------------------------------------------- */

struct GuiParameter : public ParameterLayout
{
   GuiParameter(ParameterLayout layout);
   GuiParameter() = delete;

   float value;
};

/* -------------------------------------------------------------------------- */
/*              COLLECTION OF DSP PARAMETERS                                  */
/* -------------------------------------------------------------------------- */

class DspParameters
{
   friend class GuiParameters;

public:
   void add(ParameterLayout& layout);

   void push(uint32_t paramId, float& newValue);
   void update(uint32_t paramId);

   float value(uint32_t paramId);

private:
   std::map<uint32_t, std::unique_ptr<DspParameter>> _params;
};

/* -------------------------------------------------------------------------- */
/*              COLLECTION OF GUI PARAMETERS                                  */
/* -------------------------------------------------------------------------- */

class DspParameters;

class GuiParameters
{
public:
   GuiParameters(const DspParameters& audioParameters);
   GuiParameters() = delete;

   GuiParameter* byId(uint32_t paramId);

private:
   std::map<uint32_t, std::unique_ptr<GuiParameter>> _params;
};

} // namespace ImRt
