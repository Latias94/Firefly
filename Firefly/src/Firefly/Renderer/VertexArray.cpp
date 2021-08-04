#include "ffpch.h"
#include "Firefly/Renderer/VertexArray.h"
#include "Firefly/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Firefly
{
    Ref <VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            FF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::API::OpenGL:return CreateRef<OpenGLVertexArray>();
        }
        FF_CORE_ASSERT(false, "Unknown RendererAPI!")

        return nullptr;
    }
}