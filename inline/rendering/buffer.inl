#pragma once

#include "../../include/application/errors.hpp"

#include "../../include/rendering/buffer.hpp"

template <BufferType Type, BufferMode Mode, typename T>
Buffer<Type, Mode, T>::Buffer()
{
}

template <BufferType Type, BufferMode Mode, typename T>
Buffer<Type, Mode, T>::~Buffer()
{
    if (mHandle)
    {
        GLErrorCheck(glDeleteBuffers(1, &mHandle));

        mHandle = 0;
    }
}

template <BufferType Type, BufferMode Mode, typename T>
void Buffer<Type, Mode, T>::Create()
{
    GLErrorCheck(glGenBuffers(1, &mHandle));
}

template <BufferType Type, BufferMode Mode, typename T>
void Buffer<Type, Mode, T>::Use()
{
    GLErrorCheck(glBindBuffer(static_cast<int>(Type), mHandle));
}

template <BufferType Type, BufferMode Mode, typename T>
void Buffer<Type, Mode, T>::Data(T* data, unsigned int size)
{
    mSize = size;

    Use();

    GLErrorCheck(glBufferData(static_cast<int>(Type), size * sizeof(T), data, static_cast<int>(Mode)));
}

template <BufferType Type, BufferMode Mode, typename T>
void Buffer<Type, Mode, T>::Data(const std::vector<T>& data)
{
    mSize = data.size();

    Use();

    GLErrorCheck(glBufferData(static_cast<int>(Type), data.size() * sizeof(T), data.data(), static_cast<int>(Mode)));
}

template <BufferType Type, BufferMode Mode, typename T>
void Buffer<Type, Mode, T>::Subdata(const std::vector<T>& data, unsigned int offset)
{
    Use();

    GLErrorCheck(glBufferSubData(static_cast<int>(Type), offset, data.size() * sizeof(T), data.data()));
}

template <BufferType Type, BufferMode Mode, typename T>
void Buffer<Type, Mode, T>::Subdata(T* data, unsigned int size, unsigned int offset)
{
    Use();

    GLErrorCheck(glBufferSubData(static_cast<int>(Type), offset, size * sizeof(T), data));
}
