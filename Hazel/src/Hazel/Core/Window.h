#pragma once

#include <sstream>

#include "Hazel/Core/Base.h"
#include "Hazel/Events/Event.h"

namespace hazel
{
struct WindowProps
{
    std::string title;
    uint32_t width;
    uint32_t height;

    WindowProps(const std::string& title = "Hazel Engine", uint32_t width = 1920, uint32_t height = 1080)
        : title(title),
          width(width),
          height(height)
    {
    }
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;
    virtual void onUpdate() = 0;
    virtual uint32_t getWidth() const = 0;
    virtual uint32_t getHeight() const = 0;
    //! Window attributes
    virtual void setEventCallback(const EventCallbackFn& callback) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSync() const = 0;
    virtual void* getNativeWindow() const = 0;

    static Scope<Window> create(const WindowProps& props = WindowProps());
};
}  // namespace hazel
