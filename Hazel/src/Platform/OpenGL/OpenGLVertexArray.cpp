#include "hzpch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace hazel
{
  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return GL_FLOAT;
			case ShaderDataType::Float2:   return GL_FLOAT;
			case ShaderDataType::Float3:   return GL_FLOAT;
			case ShaderDataType::Float4:   return GL_FLOAT;
			case ShaderDataType::Mat3:     return GL_FLOAT;
			case ShaderDataType::Mat4:     return GL_FLOAT;
			case ShaderDataType::Int:      return GL_INT;
			case ShaderDataType::Int2:     return GL_INT;
			case ShaderDataType::Int3:     return GL_INT;
			case ShaderDataType::Int4:     return GL_INT;
			case ShaderDataType::Bool:     return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

  OpenGLVertexArray::OpenGLVertexArray()
  {
    HZ_PROFILE_FUNCTION();

    glCreateVertexArrays(1, &m_rendererID);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    HZ_PROFILE_FUNCTION();

    glDeleteVertexArrays(1, &m_rendererID);
  }

  void OpenGLVertexArray::bind() const
  {
    HZ_PROFILE_FUNCTION();

    glBindVertexArray(m_rendererID);
  }

  void OpenGLVertexArray::unbind() const
  {
    HZ_PROFILE_FUNCTION();

    glBindVertexArray(0);
  }

  void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
  {
    HZ_PROFILE_FUNCTION();

    HZ_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(m_rendererID);

    vertexBuffer->bind();

    const auto& layout = vertexBuffer->getLayout();
    for (const auto& element : layout) {
      switch (element.m_type)
      {
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
        {
          glEnableVertexAttribArray(m_vertexBufferIndex);
          glVertexAttribPointer(
            m_vertexBufferIndex,
            element.getComponentCount(),
            ShaderDataTypeToOpenGLBaseType(element.m_type),
            element.m_normalized ? GL_TRUE : GL_FALSE,
            layout.getStride(),
            (const void*)element.m_offset
          );
          m_vertexBufferIndex++;
          break;
        }
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
        {
          uint8_t count = element.getComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(m_vertexBufferIndex);
						glVertexAttribPointer(m_vertexBufferIndex,
							count,
							ShaderDataTypeToOpenGLBaseType(element.m_type),
							element.m_normalized ? GL_TRUE : GL_FALSE,
							layout.getStride(),
							(const void*)(sizeof(float) * count * i));
						glVertexAttribDivisor(m_vertexBufferIndex, 1);
						m_vertexBufferIndex++;
					}
					break;
        }
				default:
					HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
      }
    }

    m_vertexBuffers.push_back(vertexBuffer);
  }

  void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
  {
    HZ_PROFILE_FUNCTION();

    glBindVertexArray(m_rendererID);
    indexBuffer->bind();

    m_indexBuffer = indexBuffer;
  }
}