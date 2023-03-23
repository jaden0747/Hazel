#pragma once

#include "Hazel/Core.h"

namespace hazel
{
  class HAZEL_API Input
  {
    public:
      inline static bool isKeyPressed(int keycode) { return s_instance->isKeyPressedImpl(keycode); }
      inline static float isMouseButtonPressed(int button) { return s_instance->isMouseButtonPressedImpl(button); }
      inline static float getMouseX() { return s_instance->getMouseXImpl(); }
      inline static float getMouseY() { return s_instance->getMouseYImpl(); }
      inline static std::pair<float, float> getMousePosition() { return s_instance->getMousePositionImpl(); }

    protected:
      virtual bool isKeyPressedImpl(int keycode) = 0;
      virtual bool isMouseButtonPressedImpl(int button) = 0;
      virtual float getMouseXImpl() = 0;
      virtual float getMouseYImpl() = 0;
      virtual std::pair<float, float> getMousePositionImpl() = 0;

    private:
      static Input* s_instance;
  };
}