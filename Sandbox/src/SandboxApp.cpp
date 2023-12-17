#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Sandbox : public hazel::Application
{
public:
  Sandbox() {
    pushLayer(new Sandbox2D());
  }


  ~Sandbox() {

  }


};


hazel::Application* hazel::createApplication() {
  return new Sandbox();
}