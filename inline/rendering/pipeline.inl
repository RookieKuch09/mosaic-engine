#pragma once

#include "../../include/application/errors.hpp"
#include "../../include/rendering/pipeline.hpp"

template <ShaderType Type>
void Pipeline::AttachShader(Shader<Type>& shader)
{
    GLErrorCheck(glAttachShader(mHandle, shader.mHandle));
}
