#include "dsp.h"
#include "gui.h"
#include "style.h"

int main()
{
   Dsp dsp;
   dsp.run(); // starts new dsp thread

   ImRt::GuiSettings settings;
   settings.title          = "Utility";
   settings.size           = { 485, 258 };
   settings.style          = ImRt::DefaultStyle();
   settings.style.fontSize = 13.0f;
   settings.alwaysOnTop    = true;
   settings.decorated      = true;

   Gui gui(dsp, settings);
   gui.run();

   return 0;
}
