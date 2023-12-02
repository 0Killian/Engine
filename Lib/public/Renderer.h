//
// Created by Killian on 02/12/2023.
//
#pragma once

namespace NGN
{
    using FramePacket = size_t;

    struct FrameData
    {};

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual FramePacket StartFrame(FrameData& frameData) = 0;
        virtual void EndFrame(FramePacket&& packet) = 0;
    };
}