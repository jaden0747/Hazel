#include "hzpch.h"
#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Log.h"

namespace hazel
{

  Application::Application() {

  }

  Application::~Application() {

  }

  void Application::run() {
    WindowResizeEvent e(1920, 1080);
    if (e.isInCategory(EventCategoryApplication)) {
      HZ_TRACE(e);
    }
    if (e.isInCategory(EventCategoryInput)) {
      HZ_TRACE(e);
    }
    while (true) {

    }
  }

}