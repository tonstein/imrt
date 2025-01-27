#pragma once

#include <cstddef>
#include <cstdio>
#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "imrt-font.embed"
#include "imrt-params.h"

namespace ImRt {

/* ------------------------------------------------------ */
/*                     window settings                    */
/* ------------------------------------------------------ */

struct Style
{
   ImGuiStyle gui   = ImGuiStyle();
   ImPlotStyle plot = ImPlotStyle();
   float fontSize   = 14.0f;
};

struct GuiSettings
{
   std::string title = "Default title";
   ImVec2 size       = { 1024, 768 };
   bool decorated    = true;
   bool alwaysOnTop  = false;
   ImVec4 clearColor = ImColor(22, 29, 38).Value;
   Style style;
};

/* ------------------------------------------------------ */
/*                           gui                          */
/* ------------------------------------------------------ */

template <typename Derived, typename Dsp>
class Gui
{
   template <typename, typename>
   friend class Slider;
   template <typename, typename>
   friend class Knob;

public:
   Gui() = delete;

   Gui(Dsp& dsp, GuiSettings settings = GuiSettings())
      : _settings(settings)
      , dsp(dsp)
      , parameters(dsp.parameters)
   {
      glfwSetErrorCallback(ErrorCallback);

      if (!glfwInit())
         std::exit(1);

      const char* glsl_version = "#version 130";
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

      if (!_settings.decorated)
         glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

      _window = glfwCreateWindow(
         _settings.size.x, _settings.size.y, _settings.title.c_str(), nullptr,
         nullptr
      );
      if (_window == NULL)
         std::exit(1);

      if (_settings.alwaysOnTop)
         glfwSetWindowAttrib(_window, GLFW_FLOATING, GLFW_TRUE);

      glfwMakeContextCurrent(_window);
      glfwSwapInterval(1);

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImPlot::CreateContext();

      ImGui::GetStyle()  = _settings.style.gui;
      ImPlot::GetStyle() = _settings.style.plot;

      glfwGetWindowContentScale(_window, &_scale.x, &_scale.y);
      ImGui::GetStyle().ScaleAllSizes(0.75f * _scale.y);

      ImGui_ImplGlfw_InitForOpenGL(_window, true);
      ImGui_ImplOpenGL3_Init(glsl_version);

      ImGuiIO& io = ImGui::GetIO();
      io.Fonts->Clear();
      ImFontConfig fontConfig;
      fontConfig.FontDataOwnedByAtlas = false;
      ImFont* notoMonoFont            = io.Fonts->AddFontFromMemoryTTF(
         (void*)notoMonoRegularTtf, sizeof(notoMonoRegularTtf),
         _settings.style.fontSize * _scale.x, &fontConfig
      );
      io.FontDefault = notoMonoFont;
   }

   virtual ~Gui()
   {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImPlot::DestroyContext();
      ImGui::DestroyContext();

      glfwDestroyWindow(_window);
      glfwTerminate();
   }

   void run()
   {
      onStart();

      while (!glfwWindowShouldClose(_window))
      {

         glfwPollEvents();

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();

         ImGuiIO& io = ImGui::GetIO();
         ImGui::SetNextWindowSize(io.DisplaySize);
         ImGui::SetNextWindowPos({ 0, 0 });
         if (ImGui::Begin(
                "Audioskop", nullptr,
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
             ))
         {
            onUpdate();
         }
         ImGui::End();

         ImGui::Render();
         int display_w, display_h;
         glfwGetFramebufferSize(_window, &display_w, &display_h);
         glViewport(0, 0, display_w, display_h);
         glClearColor(
            _settings.clearColor.x, _settings.clearColor.y,
            _settings.clearColor.z, _settings.clearColor.w
         );
         glClear(GL_COLOR_BUFFER_BIT);
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
         {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
         }
         glfwSwapBuffers(_window);
      }
   }

   ImVec2 scale()
   {
      return _scale;
   }

private:
   void onStart()
   {
      static_cast<Derived*>(this)->onStart();
   }

   void onUpdate()
   {
      static_cast<Derived*>(this)->onUpdate();
   }

protected:
   Dsp& dsp;
   GuiParameters parameters;

private:
   GLFWwindow* _window = nullptr;
   GuiSettings _settings;
   ImVec2 _scale;

   /* ------------------------------------------------------ */
   /*                     error callback                     */
   /* ------------------------------------------------------ */

private:
   static void ErrorCallback(int error, const char* description)
   {
      std::cerr << "Glfw Error" << error << ": " << description << std::endl;
   }
};

} // namespace ImRt