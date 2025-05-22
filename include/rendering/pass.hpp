#pragma once

#include <vector>

namespace Mosaic::Internal::Rendering
{
    class Material;
    class Pipeline;
    class Mesh;

    class RenderPass
    {
    public:
        RenderPass(std::vector<Mesh*> meshes, Material& material, Pipeline& pipeline);

    private:
        std::vector<Mesh*> mMeshes;

        Material& mMaterial;
        Pipeline& mPipeline;

        friend class Renderer;
    };
}