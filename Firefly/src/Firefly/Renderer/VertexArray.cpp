#include "ffpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Firefly
{
    VertexArray* VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:
            FF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
                return nullptr;
            case RendererAPI::OpenGL:
                return new OpenGLVertexArray();
        }
        FF_CORE_ASSERT(false, "Unknown RendererAPI!");

        return nullptr;
    }
}