#include "hzpch.h"
#include "Shader.h"

#include <glad/glad.h> // access to OpenGL
#include <glm/gtc/type_ptr.hpp>

namespace hazel
{
  Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
  {
    // create an empty vertex shader handle
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const char* source = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // compile the vertex shader
    glCompileShader(vertexShader);

    int isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
      int maxLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<char> errorLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

      // Provide the infolog in whatever manor you deem best.
      // Exit with failure.
      glDeleteShader(vertexShader); // Don't leak the shader.

      HZ_CORE_ERROR("{0}", errorLog.data());
      HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
      return;
    }

    // Create an empty fragment shader handle
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    source = (const char*)fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
      int maxLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<char> errorLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

      // we don't need the shader anymore.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);
      
      HZ_CORE_ERROR("{0}", errorLog.data());
      HZ_CORE_ASSERT(false, "Fragment shader compilation failure!");
      return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_rendererID = glCreateProgram();
    unsigned int program = m_rendererID;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    int isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

    if (isLinked == GL_FALSE)
    {
      int maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<char> errorLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);

      // We don't need the program anymore.
      glDeleteProgram(program);
      // Don't leak shaders either.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      HZ_CORE_ERROR("{0}", errorLog.data());
      HZ_CORE_ASSERT(false, "Shader link failure!");
      return;
    }

    // always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
  } 

  Shader::~Shader()
  {
    glDeleteProgram(m_rendererID);
  }

  void Shader::bind() const
  {
    glUseProgram(m_rendererID);
  }

  void Shader::unbind() const
  {
    glUseProgram(0);
  }

  void Shader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
  {
    GLint location = glGetUniformLocation(m_rendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }
}