#include "ffpch.h"
#include "Firefly/Renderer/GraphicsContext.h"
#include "Firefly/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Firefly
{
    Scope <GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            FF_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
                return nullptr;
            case RendererAPI::API::OpenGL: return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        FF_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}