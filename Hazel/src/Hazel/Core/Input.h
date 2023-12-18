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
      static bool isKeyPressed(KeyCode keycode) { return s_instance->isKeyPressedImpl(keycode); }
      static float isMouseButtonPressed(MouseCode button) { return s_instance->isMouseButtonPressedImpl(button); }
      static float getMouseX() { return s_instance->getMouseXImpl(); }
      static float getMouseY() { return s_instance->getMouseYImpl(); }
      static std::pair<float, float> getMousePosition() { return s_instance->getMousePositionImpl(); }

      static Scope<Input> create();

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