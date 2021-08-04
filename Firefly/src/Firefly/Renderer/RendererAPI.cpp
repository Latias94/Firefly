#include "ffpch.h"
#include "Firefly/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Firefly
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope <RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
            case RendererAPI::API::None:
            FF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
        }

        FF_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}