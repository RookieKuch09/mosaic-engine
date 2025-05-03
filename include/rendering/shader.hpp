#pragma once

#include <GL/glew.h>

#include <string>

enum class ShaderType
{
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

template <ShaderType Type>
class Shader
{
public:
    Shader(const char* path);
    ~Shader();

    void Create();

private:
    void Compile();

    unsigned int mHandle;

    std::string mSource;
    std::string mPath;

    friend class Pipeline;
};

#include "../../inline/rendering/shader.inl"
