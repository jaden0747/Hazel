#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace hazel
{

  class HAZEL_API Application {
  public:

    Application();

    virtual ~Application();

    void run();

    void onEvent(Event& e);

    void pushLayer(Layer* layer);

    void pushOverlay(Layer* layer);

  private:
    bool onWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_window;
    
    bool m_running = true;

    LayerStack m_layerStack;
  };

  // to be defined in CLIENT
  Application* createApplication();

}