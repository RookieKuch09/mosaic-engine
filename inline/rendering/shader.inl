#pragma once

#include "../../include/application/errors.hpp"
#include "../../include/application/logging.hpp"

#include "../../include/rendering/shader.hpp"

#include <fstream>
#include <sstream>
#include <vector>

template <ShaderType Type>
Shader<Type>::Shader(const char* path)
    : mPath(path)
{
}

template <ShaderType Type>
Shader<Type>::~Shader()
{
    if (mHandle)
    {
        GLErrorCheck(glDeleteShader(mHandle));

        mHandle = 0;
    }
}

template <ShaderType Type>
void Shader<Type>::Create()
{
    GLErrorCheck(mHandle = glCreateShader(static_cast<int>(Type)));

    if (not mHandle)
    {
        LogError("Failed to create shader");
    }

    std::ifstream file(mPath);

    if (not file)
    {
        LogError("Failed to open shader file (Path: '{}')", mPath);
    }

    std::stringstream buffer;

    buffer << file.rdbuf();
    mSource = buffer.str();

    Compile();
}

template <ShaderType Type>
void Shader<Type>::Compile()
{
    const char* source = mSource.c_str();

    GLErrorCheck(glShaderSource(mHandle, 1, &source, nullptr));
    GLErrorCheck(glCompileShader(mHandle));

    int success;

    GLErrorCheck(glGetShaderiv(mHandle, GL_COMPILE_STATUS, &success));

    if (not success)
    {
        int logLength = 0;

        GLErrorCheck(glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &logLength));

        std::vector<char> message(logLength);

        GLErrorCheck(glGetShaderInfoLog(mHandle, logLength, nullptr, message.data()));

        LogError("Shader compilation error ('{}'):\n{}", mPath, message.data());
    }
}