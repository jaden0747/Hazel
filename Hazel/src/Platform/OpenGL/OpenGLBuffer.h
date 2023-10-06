#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace hazel
{
  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
    OpenGLVertexBuffer(float* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void bind() const;
    virtual void unbind() const;

    virtual const BufferLayout& getLayout() const { return m_layout; }
    virtual void setLayout(const BufferLayout& layout) { m_layout = layout; }

  private:
    uint32_t m_rendererID;
    BufferLayout m_layout;
  };

  class OpenGLIndexBuffer : public IndexBuffer
  {
  public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() const;
    virtual void unbind() const;

    virtual uint32_t getCount() const { return m_count; }

  private:
    uint32_t m_count;
    uint32_t m_rendererID;
  };
}