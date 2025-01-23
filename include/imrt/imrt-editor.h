#include <cstddef>
#include <cstdio>
#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "imrt-noto-mono-regular.embed"

namespace ImRt {

template <typename Derived, typename Processor> class Editor {
public:
   struct Config {
      friend class Editor<Derived, Processor>;

      Config() = delete;

      Config(Processor& processor)
         : processor(processor) {};

      struct {
         std::string title = "Default title";
         ImVec2 size       = { 1024, 768 };
         bool decorated    = true;
         bool alwaysOnTop  = false;
      } window;

      struct {
         float size = 14.0f;
      } font;

      struct {
         ImGuiStyle gui   = ImGuiStyle();
         ImPlotStyle plot = ImPlotStyle();
      } style;

   private:
      Processor& processor;
   };

   Editor() = delete;

   Editor(Config config)
      : _config(config)
      , processor(config.processor)
   {
      glfwSetErrorCallback(ErrorCallback);

      if (!glfwInit())
         std::exit(1);

      const char* glsl_version = "#version 130";
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

      if (!_config.window.decorated)
         glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

      _window = glfwCreateWindow(_config.window.size.x, _config.window.size.y,
         _config.window.title.c_str(), nullptr, nullptr);
      if (_window == NULL)
         std::exit(1);

      if (_config.window.alwaysOnTop)
         glfwSetWindowAttrib(_window, GLFW_FLOATING, GLFW_TRUE);

      glfwMakeContextCurrent(_window);
      glfwSwapInterval(1);

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImPlot::CreateContext();

      ImGui::GetStyle()  = _config.style.gui;
      ImPlot::GetStyle() = _config.style.plot;

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
         _config.font.size * _scale.x, &fontConfig);
      io.FontDefault = notoMonoFont;
   }

   virtual ~Editor()
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

      while (!glfwWindowShouldClose(_window)) {

         glfwPollEvents();

         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();

         onUpdate();

         ImGui::Render();
         int display_w, display_h;
         glfwGetFramebufferSize(_window, &display_w, &display_h);
         glViewport(0, 0, display_w, display_h);
         glClear(GL_COLOR_BUFFER_BIT);
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
         glfwSwapBuffers(_window);
      }
   }

   ImVec2 scale()
   {
      return _scale;
   }

protected:
   Processor& processor;

private:
   GLFWwindow* _window = nullptr;
   Config _config;
   ImVec2 _scale;

private:
   void onStart()
   {
      static_cast<Derived*>(this)->onStart();
   }

   void onUpdate()
   {
      static_cast<Derived*>(this)->onUpdate();
   }

   static void ErrorCallback(int error, const char* description)
   {
      std::cerr << "Glfw Error" << error << ": " << description << std::endl;
   }
};

} // namespace ImRt