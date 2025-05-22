#pragma once

#include "rendering/mesh.hpp"
#include "rendering/pass.hpp"
#include "rendering/pipeline.hpp"

#include <variant>
#include <vector>

namespace Mosaic::Internal::Rendering
{
    class Renderer;

    enum class RendererVSync : Types::I32;

    enum class CommandAction
    {
        Get,
        Set,
    };

    template <CommandAction Action, typename T>
    struct RendererCommand
    {
    };

    template <>
    class RendererCommand<CommandAction::Get, RendererVSync>
    {
    public:
        RendererCommand(RendererVSync& writeback);

    private:
        RendererVSync& Writeback;
    };

    template <>
    class RendererCommand<CommandAction::Set, RendererVSync>
    {
    public:
        RendererCommand(RendererVSync request);

    private:
        RendererVSync Request;
    };

    template <>
    class RendererCommand<CommandAction::Get, Types::Vec4<Types::F32>>
    {
    public:
        RendererCommand(Types::Vec4<Types::F32>& writeback);

    private:
        Types::Vec4<Types::F32>& Writeback;
    };

    template <>
    class RendererCommand<CommandAction::Set, Types::Vec4<Types::F32>>
    {
    public:
        RendererCommand(Types::Vec4<Types::F32> request);

    private:
        Types::Vec4<Types::F32> Request;
    };

    template <>
    class RendererCommand<CommandAction::Set, RenderPass>
    {
    public:
        RendererCommand(RenderPass& pass);

    private:
        RenderPass& RenderPass;
    };

    template <>
    class RendererCommand<CommandAction::Set, Buffer>
    {
    public:
        RendererCommand(Buffer& buffer);

    private:
        Buffer& mBuffer;
    };

    template <>
    class RendererCommand<CommandAction::Set, Pipeline>
    {
    public:
        RendererCommand(Pipeline& pipeline);

    private:
        Pipeline& mPipeline;
    };

    template <>
    class RendererCommand<CommandAction::Set, Shader>
    {
    public:
        RendererCommand(Shader& shader);

    private:
        Shader& mShader;
    };

    using VSyncStateGetCommand = RendererCommand<CommandAction::Get, RendererVSync>;
    using VSyncStateSetCommand = RendererCommand<CommandAction::Set, RendererVSync>;
    using SwapColourGetCommand = RendererCommand<CommandAction::Get, Types::Vec2<Types::F32>>;
    using SwapColourSetCommand = RendererCommand<CommandAction::Set, Types::Vec2<Types::F32>>;
    using RenderPassCommand = RendererCommand<CommandAction::Set, RenderPass>;
    using BufferInteractCommand = RendererCommand<CommandAction::Set, Buffer>;
    using ShaderInteractCommand = RendererCommand<CommandAction::Set, Shader>;
    using PipelineInteractCommand = RendererCommand<CommandAction::Set, Pipeline>;

    using RendererCommandWrapper = std::variant<
        VSyncStateGetCommand,
        VSyncStateSetCommand,
        SwapColourGetCommand,
        SwapColourSetCommand,
        RenderPassCommand,
        BufferInteractCommand,
        ShaderInteractCommand,
        PipelineInteractCommand>;

    class RendererCommandManager
    {
    public:
        RendererCommandManager(Renderer& renderer);

        void Submit(const RendererCommandWrapper& command);

    private:
        void Send();
        void Purge();

        std::vector<RendererCommandWrapper> mCommands;

        Renderer& mRenderer;
    };
}