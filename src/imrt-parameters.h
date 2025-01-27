#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <cstring> // choc_...FIFO.h needs it

#include <containers/choc_VariableSizeFIFO.h>

/* ------------------------------------------------------ */
/*                        parameter                       */
/* ------------------------------------------------------ */

class Parameter {
public:
   Parameter(std::string name, float min, float max, float init);
   Parameter() = delete;
   ~Parameter();

   const char* name();
   float min();
   float max();
   float init();

   float value;

private:
   std::string _name;
   float _min, _max, _init;
};

/* ------------------------------------------------------ */
/*                       parameters                       */
/* ------------------------------------------------------ */

class AudioParameters;

class Parameters {
public:
   Parameters() = delete;
   Parameters(const AudioParameters& audioParameters);
   ~Parameters();

   std::vector<uint32_t> ids();
   Parameter* byId(uint32_t paramId);

private:
   std::vector<uint32_t> _ids;
   std::map<uint32_t, Parameter*> _params;
};

/* ------------------------------------------------------ */
/*                     audio parameter                    */
/* ------------------------------------------------------ */

class AudioParameter : public Parameter {
   friend class AudioParameters;

public:
   AudioParameter(std::string name, float min, float max, float init);
   AudioParameter() = delete;
   ~AudioParameter();

   void push(float& newValue);
   void update();

private:
   choc::fifo::VariableSizeFIFO _fifo;
};

/* ------------------------------------------------------ */
/*                     audio parameters                   */
/* ------------------------------------------------------ */

class AudioParameters {
   friend class Parameters;

public:
   AudioParameters();
   ~AudioParameters();

   uint32_t add(std::string name, float min, float max, float init);

   std::vector<uint32_t> ids();

   void push(uint32_t paramId, float& newValue);
   void update(uint32_t paramId);

   float value(uint32_t paramId);

private:
   uint32_t _nextId = 0;
   std::vector<uint32_t> _ids;
   std::map<uint32_t, std::unique_ptr<AudioParameter>> _params;
};
