#pragma once

#include "GLFW/glfw3.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

namespace hazel
{
class WindowsWindow : public Window
{
public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();
    virtual void onUpdate() override;
    unsigned int getWidth() const override
    {
        return m_data.width;
    }
    unsigned int getHeight() const override
    {
        return m_data.height;
    }

    //! Window attributes
    void setEventCallback(const EventCallbackFn& callback) override
    {
        m_data.eventCallback = callback;
    }

    void setVSync(bool enabled) override;
    bool isVSync() const override;

    virtual void* getNativeWindow() const override
    {
        return m_window;
    }

private:
    virtual void init(const WindowProps& props);
    virtual void shutdown();

private:
    GLFWwindow*            m_window;
    Scope<GraphicsContext> m_context;
    struct WindowData
    {
        std::string  title;
        unsigned int width, height;
        bool         VSync;

        EventCallbackFn eventCallback;
    };
    WindowData m_data;
};
} // namespace hazel
