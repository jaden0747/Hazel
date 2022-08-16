#pragma once

#include "Core.h"
#include "Window.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace hazel
{

  class HAZEL_API Application {
  public:

    Application();

    virtual ~Application();

    void run();

    void onEvent(Event& e);

  private:
    bool onWindowClose(WindowCloseEvent& e);

    std::unique_ptr<Window> m_window;
    
    bool m_running = true;
  };

  // to be defined in CLIENT
  Application* createApplication();

}