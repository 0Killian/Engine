//
// Created by Killian on 02/12/2023.
//
#pragma once

#include "Containers/String.h"
#include "Pipeline.h"
#include "RenderAPI.h"
#include "Containers/VertexBuffer.h"

namespace NGN
{
    using FramePacket = size_t;

    struct FrameData
    {
        InstanceBuffer constantBuffer;
    };

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual RenderAPI GetAPI() const = 0;

        virtual FramePacket StartFrame(FrameData& frameData) = 0;
        virtual void EndFrame(FramePacket packet) = 0;

        virtual size_t CreatePipeline(const String& vertexSource, const String& pixelSource) = 0;
        virtual size_t CreatePipeline(const List<uint8_t>& compiledVertex, const List<uint8_t>& compiledPixel) = 0;
        
        virtual void UpdateMeshInPipeline(size_t RendererID, size_t meshID, String name, Mesh& mesh) = 0;

        virtual size_t AddMeshInstanceInPipeline(size_t RendererID, size_t meshID, InstanceBuffer buffer) = 0;
        virtual void UpdateMeshInstanceInPipeline(size_t RendererID, size_t meshID, size_t instanceID, InstanceBuffer buffer) = 0;

        static Renderer& Get();
    };
}