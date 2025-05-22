#pragma once

namespace Mosaic::Internal::Rendering
{
    class Shader
    {
    };

    class Pipeline
    {
    };

    class ShaderDescriptor
    {
    public:
        ShaderDescriptor(Pipeline& pipeline);

    private:
        Shader& mShader;
    };

    class PipelineDescriptor
    {
    public:
        PipelineDescriptor(Pipeline& pipeline);

    private:
        Pipeline& mPipeline;
    };
}