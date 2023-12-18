#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseCodes.h"

namespace hazel
{
  class Input
  {
    protected:
      Input() = default;
    public:
      Input(const Input&) = delete;
      Input& operator= (const Input&) = delete;
      inline static bool isKeyPressed(KeyCode keycode) { return s_instance->isKeyPressedImpl(keycode); }
      inline static float isMouseButtonPressed(MouseCode button) { return s_instance->isMouseButtonPressedImpl(button); }
      inline static float getMouseX() { return s_instance->getMouseXImpl(); }
      inline static float getMouseY() { return s_instance->getMouseYImpl(); }
      inline static std::pair<float, float> getMousePosition() { return s_instance->getMousePositionImpl(); }

    protected:
      virtual bool isKeyPressedImpl(KeyCode keycode) = 0;
      virtual bool isMouseButtonPressedImpl(MouseCode button) = 0;
      virtual float getMouseXImpl() = 0;
      virtual float getMouseYImpl() = 0;
      virtual std::pair<float, float> getMousePositionImpl() = 0;

    private:
      static Scope<Input> s_instance;
  };
}