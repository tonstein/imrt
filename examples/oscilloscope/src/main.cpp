#include "dsp.h"
#include "gui.h"
#include "style.h"
#include "params.h"

int main()
{
   ImRt::DspSettings dspSettings;
   dspSettings.bufferSize = BUFFER_SIZE;

   Dsp dsp(dspSettings);
   dsp.run(); // starts new dsp thread

   ImRt::GuiSettings guiSettings;
   guiSettings.title       = "Oscillosope";
   guiSettings.size        = { 600, 300 };
   guiSettings.style       = ImRt::DefaultStyle();
   guiSettings.alwaysOnTop = true;

   Gui gui(dsp, guiSettings);
   gui.run();

   return 0;
}
