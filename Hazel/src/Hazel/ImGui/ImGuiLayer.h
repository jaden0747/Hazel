#pragma once

#include "Hazel/Core/Layer.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace hazel
{
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer(const std::string& iniFilename = "imgui.ini");
    ~ImGuiLayer() = default;
    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onEvent(Event& e) override;

    void begin();
    void end();

    void blockEvents(bool block)
    {
        m_blockEvents = block;
    }

private:
    float       m_time = 0.0f;
    std::string m_iniFilename;
    bool        m_blockEvents = true;
};
} // namespace hazel
