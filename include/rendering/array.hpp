#pragma once

#include "buffer.hpp"
#include "pipeline.hpp"

class Array
{
public:
    Array();
    ~Array();

    void Create();

    void Use();

    void Render(unsigned int primitive, Pipeline& application);

    template <BufferMode Mode, typename T>
    void AttachBuffer(Buffer<BufferType::Vertex, Mode, T>& buffer, unsigned int target, unsigned int totalLength, unsigned int attributeLength, unsigned int offset);

    template <BufferMode Mode, typename T>
    void AttachBuffer(Buffer<BufferType::Index, Mode, T>& buffer);

private:
    unsigned int mIndexBufferType;
    unsigned int mIndexBufferSize;
    unsigned int mHandle;

    template <typename T>
    unsigned int GetBufferTypeEnum();
};

#include "../../inline/rendering/array.inl"
