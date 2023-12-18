#include "hzpch.h"

#include "Hazel/Core/Window.h"

#ifdef HZ_PLATFORM_WINDOWS
  #include "Platform/Windows/WindowsWindow.h"
#endif

namespace hazel
{

  Scope<Window> Window::create(const WindowProps& props)
  {
  #ifdef HZ_PLATFORM_WINDOWS
    return createScope<WindowsWindow>(props);
  #else
    HZ_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
  #endif
  }

}
