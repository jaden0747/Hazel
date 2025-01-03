#pragma once

namespace hazel
{
enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

static uint32_t shaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return 4;
    case ShaderDataType::Float2:
        return 4 * 2;
    case ShaderDataType::Float3:
        return 4 * 3;
    case ShaderDataType::Float4:
        return 4 * 4;
    case ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case ShaderDataType::Int:
        return 4;
    case ShaderDataType::Int2:
        return 4 * 2;
    case ShaderDataType::Int3:
        return 4 * 3;
    case ShaderDataType::Int4:
        return 4 * 4;
    case ShaderDataType::Bool:
        return 1;
    }

    HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
}

struct BufferElement
{
    std::string    m_name;
    ShaderDataType m_type;
    uint32_t       m_size;
    size_t         m_offset;
    bool           m_normalized;

    BufferElement() = default;

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : m_name(name)
        , m_type(type)
        , m_size(shaderDataTypeSize(type))
        , m_offset(0)
        , m_normalized(normalized)
    {
    }

    uint32_t getComponentCount() const
    {
        switch (m_type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3; // 3* float3
        case ShaderDataType::Mat4:
            return 4; // 4* float4
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        }
        HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }
};

class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : m_elements(elements)
    {
        calculateOffsetsAndStride();
    }

    uint32_t getStride() const
    {
        return m_stride;
    }
    const std::vector<BufferElement>& getElements() const
    {
        return m_elements;
    }

    std::vector<BufferElement>::iterator begin()
    {
        return m_elements.begin();
    }
    std::vector<BufferElement>::iterator end()
    {
        return m_elements.end();
    }
    std::vector<BufferElement>::const_iterator begin() const
    {
        return m_elements.begin();
    }
    std::vector<BufferElement>::const_iterator end() const
    {
        return m_elements.end();
    }

private:
    void calculateOffsetsAndStride()
    {
        size_t offset = 0;
        m_stride      = 0;
        for (auto& element : m_elements)
        {
            element.m_offset = offset;
            offset += element.m_size;
            m_stride += element.m_size;
        }
    }

private:
    std::vector<BufferElement> m_elements;
    uint32_t                   m_stride = 0;
};

class VertexBuffer
{
public:
    virtual ~VertexBuffer() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual void setData(const void* data, uint32_t size) = 0;

    virtual const BufferLayout& getLayout() const                     = 0;
    virtual void                setLayout(const BufferLayout& layout) = 0;

    static Ref<VertexBuffer> create(uint32_t size);
    static Ref<VertexBuffer> create(float* vertices, uint32_t size);
};

/**
 * @brief Currently hazel only support 32-bit index buffers
 *
 */
class IndexBuffer
{
public:
    virtual ~IndexBuffer() = default;

    virtual void bind() const   = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;

    static Ref<IndexBuffer> create(uint32_t* indices, uint32_t count);
};
} // namespace hazel
