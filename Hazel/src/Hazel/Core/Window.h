#pragma once

#include "hzpch.h"

#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"

namespace hazel 
{
  struct WindowProps
  {
    std::string title;
    unsigned int width;
    unsigned int height;

    WindowProps(const std::string& title = "Hazel Engine",
      unsigned int width=1920,
      unsigned int height=1080)
      : title(title), width(width), height(height) 
    {
    }
  };


  class HAZEL_API Window {
  public: 
    using EventCallbackFn = std::function<void(Event&)>;

    virtual ~Window() = default;
    virtual void onUpdate() = 0;
    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;
    //! Window attributes    
    virtual void setEventCallback(const EventCallbackFn& callback) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual bool isVSync() const = 0;
    virtual void* getNativeWindow() const = 0;

    static Window* create(const WindowProps& props=WindowProps());  };
}