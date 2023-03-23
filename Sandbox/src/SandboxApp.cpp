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
      HZ_TRACE("{0}", (char)e.getKeyCode());
    }
  }

};


class Sandbox : public hazel::Application
{
public:
  Sandbox() {
    pushLayer(new ExampleLayer());
  }


  ~Sandbox() {
    
  }


};


hazel::Application* hazel::createApplication() {
  return new Sandbox();
}