#pragma once

#include <ostream>

#include "Hazel/Renderer/Framebuffer.h"

namespace hazel
{

class OpenGLFramebuffer : public Framebuffer
{
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);

    virtual ~OpenGLFramebuffer();

    void invalidate();

    void bind() override;

    void unbind() override;

    void resize(uint32_t width, uint32_t height) override;

    uint32_t getColorAttachmentRendererID() const override
    {
        return m_colorAttachment;
    }

    const FramebufferSpecification& getSpecification() const override
    {
        return m_specification;
    }

    friend std::ostream& operator<<(std::ostream& os, const OpenGLFramebuffer& rhs);

private:
    uint32_t                 m_rendererID      = 0;
    uint32_t                 m_colorAttachment = 0, m_depthAttachment = 0;
    FramebufferSpecification m_specification;
};

} // namespace hazel
