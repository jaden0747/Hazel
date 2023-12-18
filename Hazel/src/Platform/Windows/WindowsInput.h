#pragma once

#include "Hazel/Core/Input.h"

namespace hazel
{
  class WindowsInput : public Input
  {
  protected:
    virtual bool isKeyPressedImpl(KeyCode keycode) override;
    virtual bool isMouseButtonPressedImpl(MouseCode button) override;
    virtual float getMouseXImpl() override;
    virtual float getMouseYImpl() override;
    virtual std::pair<float, float> getMousePositionImpl() override;
  };
}