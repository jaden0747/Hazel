#pragma once

#include "Hazel/Core/Base.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/Event.h"
#include "Hazel/ImGui/ImGuiLayer.h"

// #include "Hazel/Renderer/Shader.h"
// #include "Hazel/Renderer/Buffer.h"
// #include "Hazel/Renderer/VertexArray.h"
// #include "Hazel/Renderer/OrthographicCamera.h"

int main(int argc, char** argv);

namespace hazel
{
class Application
{
    friend int ::main(int argc, char** argv);

public:
    Application(const std::string& name = "Hazel App");
    virtual ~Application();
    void    onEvent(Event& e);
    void    pushLayer(Layer* layer);
    void    pushOverlay(Layer* layer);
    Window& getWindow()
    {
        return *m_window;
    }
    static Application& get()
    {
        return *s_instance;
    }

    void close();

    ImGuiLayer* getImguiLayer()
    {
        return m_imguiLayer;
    }

private:
    void run();
    bool onWindowClose(WindowCloseEvent& e);
    bool onWindowResize(WindowResizeEvent& e);

private:
    std::unique_ptr<Window> m_window;
    ImGuiLayer*             m_imguiLayer;
    bool                    m_running   = true;
    bool                    m_minimized = false;
    LayerStack              m_layerStack;
    float                   m_lastFrameTime = 0.0f;

private:
    static Application* s_instance;
};

// to be defined in CLIENT
Application* createApplication();

} // namespace hazel
