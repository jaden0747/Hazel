#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

#include "hzpch.h"

namespace hazel
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height)
{
    HZ_PROFILE_FUNCTION();

    m_internalFormat = GL_RGBA8;
    m_dataFormat     = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
    glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_path(path)
{
    HZ_PROFILE_FUNCTION();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    HZ_CORE_ASSERT(data, "Failed to load image!");
    m_width  = width;
    m_height = height;

    GLenum internalFormat = 0, dataFormat = 0;
    if (channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat     = GL_RGBA;
    }
    if (channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat     = GL_RGB;
    }

    m_internalFormat = internalFormat;
    m_dataFormat     = dataFormat;

    HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
    glTextureStorage2D(m_rendererID, 1, internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);
    // glGenerateTextureMipmap(m_rendererID);

    stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    HZ_PROFILE_FUNCTION();

    glDeleteTextures(1, &m_rendererID);
}

void OpenGLTexture2D::setData(void* data, uint32_t size)
{
    HZ_PROFILE_FUNCTION();

    uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
    HZ_CORE_ASSERT(size = m_width * m_height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::bind(uint32_t slot) const
{
    HZ_PROFILE_FUNCTION();

    glBindTextureUnit(slot, m_rendererID);
}

} // namespace hazel
