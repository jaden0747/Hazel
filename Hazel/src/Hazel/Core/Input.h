#pragma once

#include "Hazel/Core/Base.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseCodes.h"

namespace hazel
{
class Input
{
public:
    static bool isKeyPressed(Key key);

    static bool isMouseButtonPressed(MouseCode button);

    static std::pair<float, float> getMousePosition();

    static float getMouseX();

    static float getMouseY();
};
} // namespace hazel
