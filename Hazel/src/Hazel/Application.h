#pragma once
#include "Core.h"
#include "Events/Event.h"
namespace hazel
{

  class HAZEL_API Application {
  public:

    Application();

    virtual ~Application();

    void run();
  };

  // to be defined in CLIENT
  Application* createApplication();

}