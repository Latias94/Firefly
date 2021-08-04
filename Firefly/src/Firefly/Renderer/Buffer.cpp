#include "ffpch.h"
#include "Firefly/Renderer/Buffer.h"
#include "Firefly/Renderer/Renderer.h"
#include <Platform/OpenGL/OpenGLBuffer.h>

#ifdef FF_PLATFORM_WINDOWS
//#include "Platform/Direct3D/Direct3DBuffer.h"
#endif

namespace Firefly
{
    Ref <VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            FF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }
        FF_CORE_ASSERT(false, "Unknown RendererAPI!")

        return nullptr;
    }


    Ref <IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            FF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:return CreateRef<OpenGLIndexBuffer>(indices, count);
        }
        FF_CORE_ASSERT(false, "Unknown RendererAPI!")

        return nullptr;
    }
}