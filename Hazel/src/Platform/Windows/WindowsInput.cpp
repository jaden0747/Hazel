#include <GLFW/glfw3.h>

#include "Hazel/Core/Application.h"
#include "Hazel/Core/Input.h"
#include "hzpch.h"

namespace hazel
{
bool Input::isKeyPressed(Key Key)
{
    auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    auto state = glfwGetKey(window, static_cast<int32_t>(Key));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed(MouseCode button)
{
    auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
    return state == GLFW_PRESS;
}

std::pair<float, float> Input::getMousePosition()
{
    auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {(float)xpos, (float)ypos};
}

float Input::getMouseX()
{
    auto [x, y] = getMousePosition();
    return x;
}

float Input::getMouseY()
{
    auto [x, y] = getMousePosition();
    return y;
}

}  // namespace hazel
