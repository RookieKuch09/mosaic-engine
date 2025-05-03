#pragma once

#include <GL/glew.h>

#include <string>

enum class TextureWrap
{
    Repeat = GL_REPEAT,
    MirroredRepeat = GL_MIRRORED_REPEAT,
    ClampToEdge = GL_CLAMP_TO_EDGE,
    ClampToBorder = GL_CLAMP_TO_BORDER,
    MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
};

enum class TextureFilter
{
    Linear = GL_LINEAR,
    Nearest = GL_NEAREST,
};

template <TextureWrap Wrap, TextureFilter Filter>
class Texture
{
public:
    Texture(const std::string path, int unit);
    ~Texture();

    void Create();

    void Use();

private:
    std::string mPath;

    unsigned int mHandle;

    int mUnit;
};

#include "../../inline/rendering/texture.inl"
