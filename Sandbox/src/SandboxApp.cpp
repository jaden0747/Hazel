#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class Sandbox : public hazel::Application
{
public:
  Sandbox()
  : Application("hazel-app")
  {
    pushLayer(new Sandbox2D());
  }


  ~Sandbox() {

  }


};


hazel::Application* hazel::createApplication() {
  return new Sandbox();
}