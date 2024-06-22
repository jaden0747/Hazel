#pragma once

#include "Hazel/Core/Input.h"
#include "Hazel/Events/Event.h"

namespace hazel
{
class KeyEvent : public Event
{
public:
    Key getKey() const
    {
        return m_Key;
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    KeyEvent(Key Key)
        : m_Key(Key)
    {
    }

    Key m_Key;
};

class KeyPressedEvent : public KeyEvent
{
public:
    KeyPressedEvent(Key Key, int repeatCount)
        : KeyEvent(Key),
          m_repeatCount(repeatCount)
    {
    }

    int getRepeatCount() const
    {
        return m_repeatCount;
    }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyPresedEvent: " << m_Key << " (" << m_repeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

private:
    int m_repeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
    KeyReleasedEvent(Key Key)
        : KeyEvent(Key)
    {
    }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_Key;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
    KeyTypedEvent(Key Key)
        : KeyEvent(Key)
    {
    }

    std::string toString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_Key;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};
}  // namespace hazel
