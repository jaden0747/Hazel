#pragma once
#include <iostream>
#include "Application.h"
#ifdef HZ_PLATFORM_WINDOWS

extern hazel::Application* hazel::createApplication();

int main(int argc, char** argv) {
  std::cout << "Starting Hazel engine" << std::endl;
  auto* app = hazel::createApplication();
  app->run();
  delete app;
  return 0;
}

#endif