#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

#include "hzpch.h"

namespace hazel
{
//!
//! VertexBuffer
//!
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
    HZ_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
{
    HZ_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    HZ_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_rendererID);
}

void OpenGLVertexBuffer::bind() const
{
    HZ_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void OpenGLVertexBuffer::unbind() const
{
    HZ_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::setData(const void* data, uint32_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

//!
//! IndexBuffer
//!
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : m_count(count)
{
    HZ_PROFILE_FUNCTION();

    glCreateBuffers(1, &m_rendererID);
    // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
    // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    HZ_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_rendererID);
}

void OpenGLIndexBuffer::bind() const
{
    HZ_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void OpenGLIndexBuffer::unbind() const
{
    HZ_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}  // namespace hazel
