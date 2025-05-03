#include "../../include/application/camera.hpp"
#include "../../include/application/window.hpp"

#include "../../include/rendering/pipeline.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <vector>

Camera::Camera(CameraData& cameraData, WindowData& surfaceData)
    : mCameraData(&cameraData), mWindowData(&surfaceData)
{
    mCameraData->Position = glm::vec2(0.0, 0.0);
    mCameraData->Projection = glm::mat4(1.0);
}

void Camera::Update()
{
    float aspect = static_cast<float>(mWindowData->Size.x) / static_cast<float>(mWindowData->Size.y);

    if (aspect >= 1.0f)
    {
        mCameraData->Projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
    }
    else
    {
        mCameraData->Projection = glm::ortho(-1.0f, 1.0f, -1.0f / aspect, 1.0f / aspect);
    }

    for (auto& pipeline : mPipelines)
    {
        pipeline->Uniform("CameraProjection", mCameraData->Projection);
        pipeline->Uniform("CameraPosition", mCameraData->Position);
    }
}

void Camera::RegisterPipeline(Pipeline* pipeline)
{
    auto it = std::find(mPipelines.begin(), mPipelines.end(), pipeline);

    if (it == mPipelines.end())
    {
        mPipelines.push_back(pipeline);
    }
}

void Camera::DeregisterPipeline(Pipeline* pipeline)
{
    auto it = std::find(mPipelines.begin(), mPipelines.end(), pipeline);

    if (it != mPipelines.end())
    {
        mPipelines.erase(it);
    }
}

std::vector<Pipeline*> Camera::mPipelines;
