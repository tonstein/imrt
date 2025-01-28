#include "gui.h"
#include "imgui.h"
#include "implot.h"
#include "params.h"
#include "style.h"

Gui::Gui(Dsp& dsp, ImRt::GuiSettings settings)
   : ImRt::Gui<Gui, Dsp>(dsp, settings)
{
}

void Gui::onStart() { }

void Gui::onUpdate()
{
   ImVec2 plotSize
      = ImGui::GetIO().DisplaySize - ImGui::GetStyle().DisplayWindowPadding;

   ImPlotFlags plotFlags = ImPlotFlags_CanvasOnly;
   ImPlotAxisFlags axisFlags
      = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Lock;

   if (ImPlot::BeginPlot("##NoTitle", plotSize, plotFlags))
   {
      ImPlot::SetupAxes(nullptr, nullptr, axisFlags, axisFlags);
      ImPlot::SetupAxisLimits(ImAxis_X1, 0, BUFFER_SIZE);
      ImPlot::SetupAxisLimits(ImAxis_Y1, -1, 1);

      float *dataL, *dataR;
      dataL = &dsp.view.getSample(0, 0);
      dataR = &dsp.view.getSample(1, 0);

      ImPlot::PlotLine("#Left", dataL, BUFFER_SIZE);

      ImPlot::PushStyleColor(ImPlotCol_Line, ImRt::Color::Blue);
      ImPlot::PlotLine("#Left", dataR, BUFFER_SIZE);
      ImPlot::PopStyleColor();
   }
   ImPlot::EndPlot();
}
