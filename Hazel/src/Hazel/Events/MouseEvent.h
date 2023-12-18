#pragma once

#include "Hazel/Events/Event.h"
#include "Hazel/Core/Input.h"

namespace hazel 
{
  class MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(float x, float y)
    : m_mouse_x(x), m_mouse_y(y) {

    }

    inline float getX() const { return m_mouse_x; }

    inline float getY() const { return m_mouse_y; }

    std::string toString() const override {
      std::stringstream ss;
      ss << "MouseMovedEvent: " << getX() << ", " << getY();
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
    float m_mouse_x;
    float m_mouse_y;
  };
  
  
  class MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(float x_offset, float y_offset)
    : m_offset_x(x_offset), m_offset_y(y_offset) {

    }

    inline float getOffsetX() const { return m_offset_x; }

    inline float getOffsetY() const { return m_offset_y; }

    std::string toString() const override {
      std::stringstream ss;
      ss << "MouseScrolledEvent: " << getOffsetX() << ", " << getOffsetY();
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  private:
    float m_offset_x;
    float m_offset_y;
  };


  class MouseButtonEvent : public Event {
  public:
    inline MouseCode getMouseButton() const { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
  protected:
    MouseButtonEvent(MouseCode button)
    : m_button(button) {
      
    }

    MouseCode m_button;
  };


  class MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    MouseButtonPressedEvent(MouseCode button)
      : MouseButtonEvent(button) {

      }

    std::string toString() const override {
      std::stringstream ss;
      ss << "MouseButtonPressedEvent: " << m_button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
  };


  class MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    MouseButtonReleasedEvent(MouseCode button)
    : MouseButtonEvent(button) {
      
    }

    std::string toString() const override {
      std::stringstream ss;
      ss << "MouseButtonReleasedEvent: " << m_button;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
  };


}
