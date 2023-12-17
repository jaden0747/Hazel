#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace hazel
{
  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case hazel::ShaderDataType::Float:    return GL_FLOAT;
			case hazel::ShaderDataType::Float2:   return GL_FLOAT;
			case hazel::ShaderDataType::Float3:   return GL_FLOAT;
			case hazel::ShaderDataType::Float4:   return GL_FLOAT;
			case hazel::ShaderDataType::Mat3:     return GL_FLOAT;
			case hazel::ShaderDataType::Mat4:     return GL_FLOAT;
			case hazel::ShaderDataType::Int:      return GL_INT;
			case hazel::ShaderDataType::Int2:     return GL_INT;
			case hazel::ShaderDataType::Int3:     return GL_INT;
			case hazel::ShaderDataType::Int4:     return GL_INT;
			case hazel::ShaderDataType::Bool:     return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

  OpenGLVertexArray::OpenGLVertexArray()
  {
    glCreateVertexArrays(1, &m_rendererID);
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    glDeleteVertexArrays(1, &m_rendererID);
  }

  void OpenGLVertexArray::bind() const
  {
    glBindVertexArray(m_rendererID);
  }

  void OpenGLVertexArray::unbind() const
  {
    glBindVertexArray(0);
  }

  void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
  {
    HZ_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

    glBindVertexArray(m_rendererID);

    vertexBuffer->bind();

    const auto& layout = vertexBuffer->getLayout();
    for (const auto& element : layout) {
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
    }

    m_vertexBuffers.push_back(vertexBuffer);
  }

  void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
  {
    glBindVertexArray(m_rendererID);
    indexBuffer->bind();

    m_indexBuffer = indexBuffer;
  }
}