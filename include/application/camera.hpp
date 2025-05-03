#pragma once

#include <glm/glm.hpp>

#include <vector>

struct WindowData;

class Pipeline;

struct CameraData
{
    glm::mat4 Projection;
    glm::vec2 Position;

    float Speed;
};

class Camera
{
public:
    Camera(CameraData& cameraData, WindowData& surfaceData);

    void Update();

private:
    static void RegisterPipeline(Pipeline* pipeline);
    static void DeregisterPipeline(Pipeline* pipeline);

    CameraData* mCameraData;
    WindowData* mWindowData;

    static std::vector<Pipeline*> mPipelines;

    friend class Pipeline;
};
