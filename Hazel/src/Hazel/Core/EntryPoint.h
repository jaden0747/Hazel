#pragma once
#include "Hazel/Core/Base.h"
#ifdef HZ_PLATFORM_WINDOWS

extern hazel::Application* hazel::createApplication();

int main(int argc, char** argv) {
  hazel::Log::init();
  HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
  auto* app = hazel::createApplication();
  HZ_PROFILE_END_SESSION();

  HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
  app->run();
  HZ_PROFILE_END_SESSION();

  HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
  delete app;
  HZ_PROFILE_END_SESSION();
  return 0;
}

#endif
