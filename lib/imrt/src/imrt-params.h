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
 * @brief This class serves as a "skeleton" for a parameter layout. Note that a
 * parameter layout does not have a current parameter value, but the derived
 * classes GuiParameter and DspParameter have. These derived classes deal with
 * the parameter value in different ways (cf. GuiParameter and DspParameter).
 */
class ParameterLayout
{
public:
   /**
    * @brief Constructs a new parameter layout object that can be added to a
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

/**
 * @brief This class fills the skeleton given by a parameter layout with life.
 * It has a parameter value to which changes can be announced via
 * DspParameter::announce(). This is typically done by the GUI thread. The
 * changes are applied when the DspParameter::updatedValue() method is called.
 * This is typically done by the DSP thread.
 */
class DspParameter : public ParameterLayout
{
public:
   /**
    * @brief Constructs a new DSP parameter object based on a ParameterLayout.
    *
    * @param layout The layout with the information for the ID, name, minimum,
    * maximum and initial value of the DSP parameter.
    */
   DspParameter(ParameterLayout layout);
   DspParameter() = delete;

   /**
    * @brief Announces a change of the parameter value by pushing the new value
    * to a multiple writer, single consumer parameter FIFO. Multiple write
    * threads may have to briefly spin-wait for each other, but the reader
    * thread is not blocked by the activity of writers.
    *
    * @param newValue The value to which the parameter value should change.
    */
   void announceChange(float& newValue);

   /**
    * @brief Updates a possible DspParameter value change by consuming the
    * value from the FIFO to which Dsp::announceChange()
    * pushes. The new value of the DspParameter can be obtained by calling the
    * parameterValue() method.
    *
    * @param paramId The ID of the parameter whose value could have changed.
    */
   float updatedValue();

   /**
    * @brief Returns the value of the DspParameter.
    */
   float value();

private:
   float _value;
   choc::fifo::VariableSizeFIFO _fifo;
};

/* -------------------------------------------------------------------------- */
/*                      GUI PARAMETER                                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief This class fills the skeleton given by a parameter layout with life.
 * It has a parameter value as a public member which can be changed directly. In
 * addition to changing the value a change should be announced to the
 * corresponding DspParameter in the DSP thread by calling
 * DspParameters<>::announce(). When adding a widget for the parameter, the
 * widget does this automatically.
 *
 */
struct GuiParameter : public ParameterLayout
{
   /**
    * @brief Constructs a new GUI parameter object.
    *
    * @param layout The layout with the information for the ID, name, minimum,
    * maximum and initial value of the GUI parameter.
    */
   GuiParameter(ParameterLayout layout);
   GuiParameter() = delete;

   /**
    * @brief The current value of the GUI parameter.
    */
   float value;
};

/* -------------------------------------------------------------------------- */
/*              COLLECTION OF DSP PARAMETERS                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief A collection of DspParameter objects that typically is owned by the
 * Dsp<> object.
 */
class DspParameters
{
   friend class GuiParameters;

public:
   /**
    * @brief Creates a new DspParameter with the given ParameterLayout and
    * adds this parameter to the DspParameters collection managed by the Dsp
    * object.
    *
    * @param layout The skeleton for the new DSP parameter consisting of an
    * ID, a name, a maximum, minimum and initial value.
    */
   void addParameter(ParameterLayout& layout);

   /**
    * @brief Announces a change of a parameter value by pushing the new value to
    * a multiple writer, single consumer parameter FIFO. Multiple write threads
    * may have to briefly spin-wait for each other, but the reader thread is not
    * blocked by the activity of writers.
    *
    * @param paramId The ID of the parameter whose value should change.
    * @param newValue The value to which the parameter value should change.
    */
   void announceChange(uint32_t paramId, float& newValue);

   /**
    * @brief Updates a possible DspParameter value change by consuming the
    * value from the FIFO to which announceChange()
    * pushes. The new value of the DspParameter can be obtained by calling the
    * parameterValue() method.
    *
    * @param paramId The ID of the parameter whose value could have changed.
    */
   float updatedValue(uint32_t paramId);

   /**
    * @brief Returns the value of a DspParameter.
    *
    * @param paramId The ID of the DspParameter.
    */
   float value(uint32_t paramId);

private:
   std::map<uint32_t, std::unique_ptr<DspParameter>> _params;
};

/* -------------------------------------------------------------------------- */
/*              COLLECTION OF GUI PARAMETERS                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief A collection of GuiParameter objects that is typically owned by the
 * Gui<> object.
 */
class GuiParameters
{
public:
   /**
    * @brief Constructs a new GUI parameter collection. The parameter layouts of
    * the GuiParameter objects in the collection are identical to the parameter
    * layouts of the DspParameters that are given in the constructor argument.
    *
    * @param audioParameters The collection of DSP Parameters whose parameter
    * layouts are used as a blueprint for the GUI parameters.
    */
   GuiParameters(const DspParameters& audioParameters);
   GuiParameters() = delete;

   /**
    * @brief Returns the GuiParameter identified by the given parameter ID.
    */
   GuiParameter* byId(uint32_t paramId);

private:
   std::map<uint32_t, std::unique_ptr<GuiParameter>> _params;
};

} // namespace ImRt
