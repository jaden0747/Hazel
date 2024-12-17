#include "Hazel/Core/Application.h"

#include <GLFW/glfw3.h>

#include "Hazel/Core/Input.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Renderer/Renderer.h"
#include "hzpch.h"

namespace hazel
{

Application* Application::s_instance = nullptr;

Application::Application(const std::string& name)
{
    HZ_PROFILE_FUNCTION();

    HZ_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;
    m_window = (Window::create(WindowProps(name)));
    m_window->setEventCallback(HZ_BIND_EVENT_FN(Application::onEvent));

    Renderer::init();

    m_imguiLayer = new ImGuiLayer(name + ".ini");
    pushOverlay(m_imguiLayer);
}

Application::~Application()
{
    HZ_PROFILE_FUNCTION();
    Renderer::shutdown();
}

void Application::pushLayer(Layer* layer)
{
    HZ_PROFILE_FUNCTION();

    m_layerStack.pushLayer(layer);
    layer->onAttach();
}

void Application::pushOverlay(Layer* layer)
{
    HZ_PROFILE_FUNCTION();

    m_layerStack.pushOverlay(layer);
    layer->onAttach();
}

void Application::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::onWindowClose));
    dispatcher.dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::onWindowResize));
    for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
    {
        if (e.m_handled)
        {
            break;
        }
        (*it)->onEvent(e);
    }
}

void Application::run()
{
    HZ_PROFILE_FUNCTION();

    while (m_running)
    {
        HZ_PROFILE_SCOPE("RunLoop");
        float time = (float)glfwGetTime();  // Platform::GetTime()
        Timestep timestep = time - m_lastFrameTime;
        m_lastFrameTime = time;

        if (!m_minimized)
        {
            {
                HZ_PROFILE_SCOPE("LayerStack OnUpdate");

                for (Layer* layer : m_layerStack)
                {
                    layer->onUpdate(timestep);
                }
            }
            m_imguiLayer->begin();
            {
                HZ_PROFILE_SCOPE("LayerStack OnImGuiRender");
                for (Layer* layer : m_layerStack) layer->onImGuiRender();
            }
            m_imguiLayer->end();
        }
        m_window->onUpdate();
    }
}

void Application::close()
{
    m_running = false;
}

bool Application::onWindowClose(WindowCloseEvent& e)
{
    m_running = false;
    return true;
}

bool Application::onWindowResize(WindowResizeEvent& e)
{
    HZ_PROFILE_FUNCTION();

    if (e.getWidth() == 0 || e.getHeight() == 0)
    {
        m_minimized = true;
        return false;
    }

    m_minimized = false;
    Renderer::onWindowResize(e.getWidth(), e.getHeight());
    return false;
}

}  // namespace hazel
