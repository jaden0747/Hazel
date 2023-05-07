#pragma once

#include "Hazel/Renderer/VertexArray.h"

namespace hazel
{
  class OpenGLVertexArray : public VertexArray
  {
  public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    virtual void bind() const override;
    virtual void unbind() const override;

    virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
    virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

    virtual const std::vector<std::shared_ptr<VertexBuffer>>& getVertexBuffers() const { return m_vertexBuffers; }
    virtual const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_indexBuffer; }

  private:
    uint32_t m_rendererID;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
  };
}