#pragma once

#include "rendering/pass.hpp"
#include "rendering/target.hpp"

#include <vector>

namespace Mosaic::Internal::Rendering
{
    class GlobalRenderGraph
    {
    public:
        void AddRenderTarget(RenderTarget& target);

    private:
        void Send();
        void Purge();

        std::vector<RenderTarget*> mTargets;
    };

    class LocalRenderGraph
    {
    public:
        void AddRenderPass(RenderPass& pass);

    private:
        void Send();
        void Purge();

        std::vector<RenderTarget*> mPasses;
    };
}