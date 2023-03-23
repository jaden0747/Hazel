#include <Hazel.h>


class ExampleLayer : public hazel::Layer {
public:

  ExampleLayer()
  : Layer("Example") {
    
  }


  void onUpdate() override {
    if (hazel::Input::isKeyPressed(HZ_KEY_TAB)) {
      HZ_TRACE("Tab key is pressed (poll)!");
    }
  }


  void onEvent(hazel::Event& event) override {
    if (event.getEventType() == hazel::EventType::KeyPressed) {
      hazel::KeyPressedEvent& e = (hazel::KeyPressedEvent&)event;
      if (e.getKeyCode() == HZ_KEY_TAB) {
        HZ_TRACE("Tab key is pressed (event)!");
      }
    }
  }

};


class Sandbox : public hazel::Application
{
public:
  Sandbox() {
    // pushLayer(new ExampleLayer());
    pushOverlay(new hazel::ImGuiLayer());
  }


  ~Sandbox() {
    
  }


};


hazel::Application* hazel::createApplication() {
  return new Sandbox();
}