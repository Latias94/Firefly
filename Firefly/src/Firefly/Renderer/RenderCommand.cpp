#include "ffpch.h"
#include "RenderCommand.h"
#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace Firefly
{
    Scope <RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}
