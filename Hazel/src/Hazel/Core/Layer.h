#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Timestep.h"

namespace hazel 
{
  class Layer {
  public:
    Layer(const std::string& name="Layer");    
    virtual ~Layer() = default;
    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void onUpdate(Timestep ts) {}
    virtual void onImGuiRender() {}
    virtual void onEvent(Event& event) {}
    inline const std::string& getName() const { return m_debugName; }
    
  protected:
    std::string m_debugName;
  };

}