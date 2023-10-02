#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace hazel 
{
  class HAZEL_API ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    ~ImGuiLayer() = default;
    virtual void onAttach() override;
    virtual void onDetach() override;
    virtual void onImGuiRender() override;
    virtual void onEvent(Event& e) override;

    void begin();
    void end();

  private:
    float m_time = 0.0f;
  };
}