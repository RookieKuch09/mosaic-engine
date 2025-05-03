#pragma once

#include "../../include/application/errors.hpp"

#include "../../include/rendering/array.hpp"

template <BufferMode Mode, typename T>
void Array::AttachBuffer(Buffer<BufferType::Vertex, Mode, T>& buffer, unsigned int target, unsigned int totalLength, unsigned int attributeLength, unsigned int offset)
{
    Use();
    buffer.Use();

    GLErrorCheck(glEnableVertexAttribArray(target));
    GLErrorCheck(glVertexAttribPointer(target, attributeLength, GetBufferTypeEnum<T>(), GL_FALSE, totalLength * sizeof(T), (void*)(offset * sizeof(T))));
}

template <BufferMode Mode, typename T>
void Array::AttachBuffer(Buffer<BufferType::Index, Mode, T>& buffer)
{
    Use();
    buffer.Use();

    mIndexBufferSize = buffer.mSize;
    mIndexBufferType = GetBufferTypeEnum<T>();
}

template <>
constexpr unsigned int Array::GetBufferTypeEnum<float>()
{
    return GL_FLOAT;
}

template <>
constexpr unsigned int Array::GetBufferTypeEnum<double>()
{
    return GL_DOUBLE;
}

template <>
constexpr unsigned int Array::GetBufferTypeEnum<int>()
{
    return GL_INT;
}

template <>
constexpr unsigned int Array::GetBufferTypeEnum<unsigned int>()
{
    return GL_UNSIGNED_INT;
}
