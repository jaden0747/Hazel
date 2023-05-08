#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/ImGui/ImGuiLayer.h"

// #include "Hazel/Renderer/Shader.h"
// #include "Hazel/Renderer/Buffer.h"
// #include "Hazel/Renderer/VertexArray.h"
// #include "Hazel/Renderer/OrthographicCamera.h"

namespace hazel
{

  class Application {
  public:
    Application();
    virtual ~Application() = default;
    void run();
    void onEvent(Event& e);
    void pushLayer(Layer* layer);
    void pushOverlay(Layer* layer);
    inline Window& getWindow() { return *m_window; }
    inline static Application& get() { return *s_instance; }

  private:
    bool onWindowClose(WindowCloseEvent& e);

  private:
    std::unique_ptr<Window> m_window;
		ImGuiLayer* m_ImGuiLayer;
    bool m_running = true;
    LayerStack m_layerStack;
    float m_lastFrameTime = 0.0f;

  private:
    static Application* s_instance;
  };

  // to be defined in CLIENT
  Application* createApplication();

}