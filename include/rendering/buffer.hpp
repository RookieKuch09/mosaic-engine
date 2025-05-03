#pragma once

#include <GL/glew.h>

#include <vector>

enum class BufferType
{
    Vertex = GL_ARRAY_BUFFER,
    Index = GL_ELEMENT_ARRAY_BUFFER
};

enum class BufferMode
{
    Static = GL_STATIC_DRAW,
    Dynamic = GL_DYNAMIC_DRAW,
};

template <BufferType Type, BufferMode Mode, typename T>
class Buffer
{
public:
    Buffer();
    ~Buffer();

    void Create();

    void Use();

    void Data(const std::vector<T>& data);
    void Data(T* data, unsigned int size);
    void Subdata(const std::vector<T>& data, unsigned int offset);
    void Subdata(T* data, unsigned int size, unsigned int offset);

private:
    unsigned int mSize;
    unsigned int mHandle;

    friend class Array;
};

#include "../../inline/rendering/buffer.inl"
