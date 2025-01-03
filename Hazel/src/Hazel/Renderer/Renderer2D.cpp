#include "Hazel/Renderer/Renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/VertexArray.h"
#include "hzpch.h"

namespace hazel
{

struct QuadVertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float     texIndex;
    float     tilingFactor;
};

struct Renderer2DData
{
    static const uint32_t maxQuads        = 10000u;
    static const uint32_t maxVertices     = maxQuads * 4u;
    static const uint32_t maxIndices      = maxQuads * 6u;
    static const uint32_t maxTextureSlots = 32u; // TODO: RenderCaps

    Ref<VertexArray>  quadVertexArray;
    Ref<VertexBuffer> quadVertexBuffer;
    Ref<Shader>       textureShader;
    Ref<Texture2D>    whiteTexture;

    uint32_t    quadIndexCount       = 0;
    QuadVertex* quadVertexBufferBase = nullptr;
    QuadVertex* quadVertexBufferPtr  = nullptr;

    std::array<Ref<Texture2D>, maxTextureSlots> textureSlots;
    uint32_t                                    textureSlotIndex = 1; // 0 = white texture

    glm::vec4 quadVertexPositions[4];

    Renderer2D::Statistics stats;
};

static Renderer2DData s_data;

void Renderer2D::init()
{
    HZ_PROFILE_FUNCTION();

    s_data.quadVertexArray  = VertexArray::create();
    s_data.quadVertexBuffer = VertexBuffer::create(s_data.maxVertices * sizeof(QuadVertex));
    s_data.quadVertexBuffer->setLayout(
        {{ShaderDataType::Float3, "a_Position"},
         {ShaderDataType::Float4, "a_Color"},
         {ShaderDataType::Float2, "a_TexCoord"},
         {ShaderDataType::Float, "a_TexIndex"},
         {ShaderDataType::Float, "a_TilingFactor"}});
    s_data.quadVertexArray->addVertexBuffer(s_data.quadVertexBuffer);

    s_data.quadVertexBufferBase = new QuadVertex[s_data.maxVertices];

    uint32_t* quadIndices = new uint32_t[s_data.maxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.maxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;
        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;
        offset += 4;
    }

    Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_data.maxIndices);

    s_data.quadVertexArray->setIndexBuffer(quadIB);
    delete[] quadIndices;

    s_data.whiteTexture       = Texture2D::create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_data.whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

    int32_t samplers[s_data.maxTextureSlots];
    for (uint32_t i = 0; i < s_data.maxTextureSlots; i++)
        samplers[i] = i;

    s_data.textureShader = Shader::create("resources/assets/shaders/Texture.glsl");
    s_data.textureShader->bind();
    s_data.textureShader->setIntArray("u_Textures", samplers, s_data.maxTextureSlots);

    // set all texture slots to 0
    s_data.textureSlots[0] = s_data.whiteTexture;

    s_data.quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    s_data.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    s_data.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    s_data.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::shutdown()
{
    HZ_PROFILE_FUNCTION();

    delete[] s_data.quadVertexBufferBase;
}

void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
{
    HZ_PROFILE_FUNCTION();

    glm::mat4 viewProj = camera.getProjection() * glm::inverse(transform);

    s_data.textureShader->bind();
    s_data.textureShader->setMat4("u_ViewProjection", viewProj);

    s_data.quadIndexCount      = 0;
    s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

    s_data.textureSlotIndex = 1;
}

void Renderer2D::beginScene(const OrthographicCamera& camera)
{
    HZ_PROFILE_FUNCTION();

    s_data.textureShader->bind();
    s_data.textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

    s_data.quadIndexCount      = 0;
    s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

    s_data.textureSlotIndex = 1;
}

void Renderer2D::endScene()
{
    HZ_PROFILE_FUNCTION();

    uint32_t dataSize = (uint32_t)((uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase);
    s_data.quadVertexBuffer->setData(s_data.quadVertexBufferBase, dataSize);

    flush();
}

void Renderer2D::flush()
{
    if (s_data.quadIndexCount == 0)
        return;

    for (uint32_t i = 0u; i < s_data.textureSlotIndex; i++)
        s_data.textureSlots[i]->bind(i);

    RenderCommand::drawIndexed(s_data.quadVertexArray, s_data.quadIndexCount);
    s_data.stats.drawCalls++;
}

void Renderer2D::flushAndReset()
{
    endScene();

    s_data.quadIndexCount      = 0;
    s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;

    s_data.textureSlotIndex = 1;
}

void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
    drawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
    HZ_PROFILE_FUNCTION();

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    drawQuad(transform, color);
}

void Renderer2D::drawQuad(
    const glm::vec2&      position,
    const glm::vec2&      size,
    const Ref<Texture2D>& texture,
    float                 tilingFactor,
    const glm::vec4&      tintColor)
{
    drawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor, tintColor);
}

void Renderer2D::drawQuad(
    const glm::vec3&      position,
    const glm::vec2&      size,
    const Ref<Texture2D>& texture,
    float                 tilingFactor,
    const glm::vec4&      tintColor)
{
    HZ_PROFILE_FUNCTION();

    glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    drawQuad(transform, texture, tilingFactor, tintColor);
}

void Renderer2D::drawRotatedQuad(
    const glm::vec2& position,
    const glm::vec2& size,
    float            rotation,
    const glm::vec4& color)
{
    drawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
}

void Renderer2D::drawRotatedQuad(
    const glm::vec3& position,
    const glm::vec2& size,
    float            rotation,
    const glm::vec4& color)
{
    HZ_PROFILE_FUNCTION();

    if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
        flushAndReset();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    drawQuad(transform, color);
}

void Renderer2D::drawRotatedQuad(
    const glm::vec2&      position,
    const glm::vec2&      size,
    float                 rotation,
    const Ref<Texture2D>& texture,
    float                 tilingFactor,
    const glm::vec4&      tintColor)
{
    drawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor, tintColor);
}

void Renderer2D::drawRotatedQuad(
    const glm::vec3&      position,
    const glm::vec2&      size,
    float                 rotation,
    const Ref<Texture2D>& texture,
    float                 tilingFactor,
    const glm::vec4&      tintColor)
{
    HZ_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation), {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    drawQuad(transform, texture, tilingFactor, tintColor);
}

void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color)
{
    HZ_PROFILE_FUNCTION();

    constexpr size_t    quadVertexCount = 4u;
    const float         textureIndex    = 0.0f; // white texture
    constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
    const float         tilingFactor    = 1.0f;

    if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
        flushAndReset();

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.quadVertexBufferPtr->position     = transform * s_data.quadVertexPositions[i];
        s_data.quadVertexBufferPtr->color        = color;
        s_data.quadVertexBufferPtr->texCoord     = textureCoords[i];
        s_data.quadVertexBufferPtr->texIndex     = textureIndex;
        s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_data.quadVertexBufferPtr++;
    }

    s_data.quadIndexCount += 6;
    s_data.stats.quadCount++;
}

void Renderer2D::drawQuad(
    const glm::mat4&      transform,
    const Ref<Texture2D>& texture,
    float                 tilingFactor,
    const glm::vec4&      tintColor)
{
    HZ_PROFILE_FUNCTION();

    constexpr size_t    quadVertexCount = 4u;
    constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    if (s_data.quadIndexCount >= Renderer2DData::maxIndices)
        flushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1u; i < s_data.textureSlotIndex; i++)
    {
        if (*s_data.textureSlots[i].get() == *texture.get())
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        if (s_data.textureSlotIndex >= Renderer2DData::maxTextureSlots)
            flushAndReset();
        textureIndex                                 = (float)s_data.textureSlotIndex;
        s_data.textureSlots[s_data.textureSlotIndex] = texture;
        s_data.textureSlotIndex++;
    }

    for (size_t i = 0; i < quadVertexCount; i++)
    {
        s_data.quadVertexBufferPtr->position     = transform * s_data.quadVertexPositions[i];
        s_data.quadVertexBufferPtr->color        = tintColor;
        s_data.quadVertexBufferPtr->texCoord     = textureCoords[i];
        s_data.quadVertexBufferPtr->texIndex     = textureIndex;
        s_data.quadVertexBufferPtr->tilingFactor = tilingFactor;
        s_data.quadVertexBufferPtr++;
    }

    s_data.quadIndexCount += 6;
    s_data.stats.quadCount++;
}

void Renderer2D::resetStats()
{
    memset(&s_data.stats, 0, sizeof(Statistics));
}

Renderer2D::Statistics Renderer2D::getStats()
{
    return s_data.stats;
}

} // namespace hazel
