#include <cstddef>
#include <cstdio>
#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>

#include "imrt-noto-mono-regular.embed"

template <typename Derived> class ImRtEditor {
public:
   ImRtEditor(std::string title, ImVec2 windowSize, float fontSize,
      ImGuiStyle guiStyle = ImGuiStyle(), ImPlotStyle plotStyle = ImPlotStyle(),
      bool windowDecoration = true, bool alwaysOnTop = false)
      : _title(title)
      , _windowSize(windowSize)
      , _fontSize(fontSize)
   {
      glfwSetErrorCallback(ErrorCallback);

      if (!glfwInit())
         std::exit(1);

      const char* glsl_version = "#version 130";
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

      if (!windowDecoration)
         glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

      _window = glfwCreateWindow(
         windowSize.x, windowSize.y, title.c_str(), nullptr, nullptr);
      if (_window == NULL)
         std::exit(1);

      if (alwaysOnTop)
         glfwSetWindowAttrib(_window, GLFW_FLOATING, GLFW_TRUE);

      glfwMakeContextCurrent(_window);
      glfwSwapInterval(1);

      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImPlot::CreateContext();

      ImGui::GetStyle()  = guiStyle;
      ImPlot::GetStyle() = plotStyle;

      glfwGetWindowContentScale(_window, &_scale.x, &_scale.y);
      ImGui::GetStyle().ScaleAllSizes(0.75f * _scale.y);

      ImGui_ImplGlfw_InitForOpenGL(_window, true);
      ImGui_ImplOpenGL3_Init(glsl_version);

      io = &ImGui::GetIO();
      io->Fonts->Clear();
      // io->Fonts->AddFontFromFileTTF(_font.c_str(), fontSize * _scale.x);
      ImFontConfig fontConfig;
      fontConfig.FontDataOwnedByAtlas = false;
      ImFont* notoMonoFont
         = io->Fonts->AddFontFromMemoryTTF((void*)notoMonoRegularTtf,
            sizeof(notoMonoRegularTtf), fontSize * _scale.x, &fontConfig);
      io->FontDefault = notoMonoFont;
   }

   virtual ~ImRtEditor()
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

         ImGui::SetNextWindowSize(io->DisplaySize);
         ImGui::SetNextWindowPos({ 0, 0 });

         ImGui::Begin("main", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
         onUpdate();
         ImGui::End();

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

private:
   GLFWwindow* _window = nullptr;
   ImVec2 _scale;

   ImGuiIO* io;

   std::string _font { "/usr/share/fonts/truetype/noto/NotoMono-Regular.ttf" };

   std::string _title = "Default";
   ImVec2 _windowSize = { 1200, 800 };
   float _fontSize    = 15;

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