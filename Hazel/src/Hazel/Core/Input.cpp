#include "hzpch.h"

#include "Hazel/Core/Input.h"

#ifdef HZ_PLATFORM_WINDOWS
  #include "Platform/Windows/WindowsInput.h"
#endif

namespace hazel
{
  Scope<Input> Input::s_instance = Input::create();

  Scope<Input> Input::create()
  {
  #ifdef HZ_PLATFORM_WINDOWS
    return createScope<WindowsInput>();
  #else
    HZ_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
  #endif
  }
}
