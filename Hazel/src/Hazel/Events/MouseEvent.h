#pragma once

#include "Event.h"

namespace hazel 
{
    class HAZEL_API MouseMovedEvent : public Event {
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


    class HAZEL_API MouseScrolledEvent : public Event {
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

    private:
        float m_offset_x;
        float m_offset_y;
    };
}