#include "../include/rendering/pipeline.hpp"
#include "../include/application/camera.hpp"

#include <glm/gtc/type_ptr.hpp>

Pipeline::Pipeline()
    : mFollowCamera(true)
{
    Camera::RegisterPipeline(this);
}

Pipeline::~Pipeline()
{
    Camera::DeregisterPipeline(this);

    if (mHandle)
    {
        GLErrorCheck(glDeleteApplication(mHandle));

        mHandle = 0;
    }
}

void Pipeline::Create()
{
    GLErrorCheck(mHandle = glCreateApplication());
}

void Pipeline::Link()
{
    GLErrorCheck(glLinkApplication(mHandle));

    int isLinked = 0;

    glGetApplicationiv(mHandle, GL_LINK_STATUS, &isLinked);

    if (not isLinked)
    {
        int maxLength = 0;

        glGetApplicationiv(mHandle, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> infoLog(maxLength);

        glGetApplicationInfoLog(mHandle, maxLength, &maxLength, infoLog.data());

        LogError("Application linking failed (Handle: {}):\n{}", mHandle, infoLog.data());

        glDeleteApplication(mHandle);

        mHandle = 0;
    }
}

void Pipeline::Use()
{
    GLErrorCheck(glUseApplication(mHandle));
}

void Pipeline::SetFollowCamera(bool should)
{
    mFollowCamera = should;
}

bool Pipeline::GetFollowCamera()
{
    return mFollowCamera;
}

void Pipeline::Uniform(const char* target, int value)
{
    GLErrorCheck(glUniform1i(GetUniformLocation(target), value));
}

void Pipeline::Uniform(const char* target, glm::ivec2 value)
{
    GLErrorCheck(glUniform2i(GetUniformLocation(target), value.x, value.y));
}

void Pipeline::Uniform(const char* target, glm::ivec3 value)
{
    GLErrorCheck(glUniform3i(GetUniformLocation(target), value.x, value.y, value.z));
}

void Pipeline::Uniform(const char* target, glm::ivec4 value)
{
    GLErrorCheck(glUniform4i(GetUniformLocation(target), value.x, value.y, value.z, value.w));
}

void Pipeline::Uniform(const char* target, unsigned int value)
{
    GLErrorCheck(glUniform1ui(GetUniformLocation(target), value));
}

void Pipeline::Uniform(const char* target, glm::uvec2 value)
{
    GLErrorCheck(glUniform2ui(GetUniformLocation(target), value.x, value.y));
}

void Pipeline::Uniform(const char* target, glm::uvec3 value)
{
    GLErrorCheck(glUniform3ui(GetUniformLocation(target), value.x, value.y, value.z));
}

void Pipeline::Uniform(const char* target, glm::uvec4 value)
{
    GLErrorCheck(glUniform4ui(GetUniformLocation(target), value.x, value.y, value.z, value.w));
}

void Pipeline::Uniform(const char* target, float value)
{
    GLErrorCheck(glUniform1f(GetUniformLocation(target), value));
}

void Pipeline::Uniform(const char* target, glm::fvec2 value)
{
    GLErrorCheck(glUniform2f(GetUniformLocation(target), value.x, value.y));
}

void Pipeline::Uniform(const char* target, glm::fvec3 value)
{
    GLErrorCheck(glUniform3f(GetUniformLocation(target), value.x, value.y, value.z));
}

void Pipeline::Uniform(const char* target, glm::fvec4 value)
{
    GLErrorCheck(glUniform4f(GetUniformLocation(target), value.x, value.y, value.z, value.w));
}

void Pipeline::Uniform(const char* target, glm::fmat4 value)
{
    GLErrorCheck(glUniformMatrix4fv(GetUniformLocation(target), 1, GL_FALSE, glm::value_ptr(value)));
}

unsigned int Pipeline::GetUniformLocation(const char* target)
{
    Use();

    GLErrorCheck(unsigned int location = glGetUniformLocation(mHandle, target));

    if (location == -1)
    {
        LogError("Failed to find uniform location: '{}'", target);
    }

    return location;
}
