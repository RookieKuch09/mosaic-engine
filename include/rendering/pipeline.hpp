#pragma once

#include "shader.hpp"

#include <glm/glm.hpp>

class Pipeline
{
public:
    Pipeline();
    ~Pipeline();

    void Create();

    void Use();

    void Link();

    template <ShaderType Type>
    void AttachShader(Shader<Type>& shader);

    void SetFollowCamera(bool should);
    bool GetFollowCamera();

    void Uniform(const char* target, int value);
    void Uniform(const char* target, glm::ivec2 value);
    void Uniform(const char* target, glm::ivec3 value);
    void Uniform(const char* target, glm::ivec4 value);

    void Uniform(const char* target, unsigned int value);
    void Uniform(const char* target, glm::uvec2 value);
    void Uniform(const char* target, glm::uvec3 value);
    void Uniform(const char* target, glm::uvec4 value);

    void Uniform(const char* target, float value);
    void Uniform(const char* target, glm::fvec2 value);
    void Uniform(const char* target, glm::fvec3 value);
    void Uniform(const char* target, glm::fvec4 value);
    void Uniform(const char* target, glm::fmat4 value);

private:
    unsigned int GetUniformLocation(const char* target);

    unsigned int mHandle;

    bool mLinked;
    bool mFollowCamera;
};

#include "../../inline/rendering/pipeline.inl"
