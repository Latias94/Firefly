#pragma once

#include "Firefly/Core/Core.h"

namespace Firefly
{
    struct FramebufferSpecification
    {
        uint32_t Width, Height;
        uint32_t Samples = 1;

        // is render to swapchain
        bool SwapChainTarget = false;
    };

    class Framebuffer
    {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual uint32_t GetColorAttachmentRendererID() const = 0;

        virtual const FramebufferSpecification& GetSpecification() const = 0;

        static Ref <Framebuffer> Create(const FramebufferSpecification& spec);
    };
}

