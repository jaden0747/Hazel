#pragma once

#include "Hazel/Renderer/Camera.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Texture.h"

namespace hazel
{
class Renderer2D
{
public:
    static void init();
    static void shutdown();

    static void beginScene(const Camera& camera, const glm::mat4& transform);
    static void beginScene(const OrthographicCamera& camera); // TODO: remove
    static void endScene();
    static void flush();

    // Primitives
    static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
    static void drawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor = 1.0f,
        const glm::vec4&      tintColor    = glm::vec4(1.0f));
    static void drawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor = 1.0f,
        const glm::vec4&      tintColor    = glm::vec4(1.0f));

    static void drawQuad(const glm::mat4& transform, const glm::vec4& color);
    static void drawQuad(
        const glm::mat4&      transform,
        const Ref<Texture2D>& texture,
        float                 tilingFactor = 1.0f,
        const glm::vec4&      tintColor    = glm::vec4(1.0f));

    static void
    drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void
    drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
    static void drawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 rotation,
        const Ref<Texture2D>& texture,
        float                 tilingFactor = 1.0f,
        const glm::vec4&      tintColor    = glm::vec4(1.0f));
    static void drawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 rotation,
        const Ref<Texture2D>& texture,
        float                 tilingFactor = 1.0f,
        const glm::vec4&      tintColor    = glm::vec4(1.0f));

    // Stats
    struct Statistics
    {
        uint32_t drawCalls = 0;
        uint32_t quadCount = 0;

        uint32_t getTotalVertexCount()
        {
            return quadCount * 4;
        }
        uint32_t getTotalIndexCount()
        {
            return quadCount * 6;
        }
    };

    static void       resetStats();
    static Statistics getStats();

private:
    static void flushAndReset();
};
} // namespace hazel
