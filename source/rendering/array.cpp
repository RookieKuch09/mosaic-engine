#include "../../include/application/errors.hpp"

#include "../../include/rendering/array.hpp"

Array::Array()
{
}

Array::~Array()
{
    if (mHandle)
    {
        GLErrorCheck(glDeleteVertexArrays(1, &mHandle));

        mHandle = 0;
    }
}

void Array::Create()
{
    GLErrorCheck(glGenVertexArrays(1, &mHandle));
}

void Array::Use()
{
    GLErrorCheck(glBindVertexArray(mHandle));
}

void Array::Render(unsigned int primitive, Pipeline& application)
{
    Use();
    application.Use();

    GLErrorCheck(glDrawElements(primitive, mIndexBufferSize, mIndexBufferType, nullptr));
}
