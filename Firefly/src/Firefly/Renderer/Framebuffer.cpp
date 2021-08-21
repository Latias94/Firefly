#include "ffpch.h"
#include "Firefly/Renderer/Framebuffer.h"
#include "Firefly/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Firefly
{
    Ref <Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            FF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:return CreateRef<OpenGLFramebuffer>(spec);
        }
        FF_CORE_ASSERT(false, "Unknown RendererAPI!")

        return nullptr;
    }
}