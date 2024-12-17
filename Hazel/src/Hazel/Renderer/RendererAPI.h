#pragma once

#include <glm/glm.hpp>

#include "Hazel/Renderer/VertexArray.h"

namespace hazel
{
class RendererAPI
{
public:
    enum class API
    {
        None = 0,
        OpenGL = 1
    };

public:
    virtual ~RendererAPI() = default;

    virtual void init() = 0;
    virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void setClearColor(const glm::vec4& color) = 0;
    virtual void clear() = 0;

    virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0u) = 0;
    static API getAPI()
    {
        return s_API;
    }

    static Scope<RendererAPI> create();

private:
    static API s_API;
};

}  // namespace hazel
