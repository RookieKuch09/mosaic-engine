#pragma once

#include "../../include/application/errors.hpp"
#include "../../include/application/logging.hpp"

#include "../../include/dependencies/stb.hpp"

#include "../../include/rendering/texture.hpp"

template <TextureWrap Wrap, TextureFilter Filter>
Texture<Wrap, Filter>::Texture(const std::string path, int unit)
    : mPath(path), mUnit(unit)
{
}

template <TextureWrap Wrap, TextureFilter Filter>
Texture<Wrap, Filter>::~Texture()
{
    if (mHandle != 0)
    {
        glDeleteTextures(1, &mHandle);
    }
}

template <TextureWrap Wrap, TextureFilter Filter>
void Texture<Wrap, Filter>::Create()
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(mPath.c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr)
    {
        LogError("Failed to load texture: " + mPath);
    }

    GLErrorCheck(glGenTextures(1, &mHandle));
    GLErrorCheck(glBindTexture(GL_TEXTURE_2D, mHandle));

    GLErrorCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(Wrap)));
    GLErrorCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(Wrap)));
    GLErrorCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, static_cast<int>(Wrap)));

    GLErrorCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(Filter)));
    GLErrorCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(Filter)));

    unsigned int format = (nrChannels == 1) ? GL_RED : (nrChannels == 3) ? GL_RGB
                                                                         : GL_RGBA;

    GLErrorCheck(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));

    GLErrorCheck(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(data);
}

template <TextureWrap Wrap, TextureFilter Filter>
void Texture<Wrap, Filter>::Use()
{
    GLErrorCheck(glActiveTexture(GL_TEXTURE0 + mUnit));
    GLErrorCheck(glBindTexture(GL_TEXTURE_2D, mHandle));
}
