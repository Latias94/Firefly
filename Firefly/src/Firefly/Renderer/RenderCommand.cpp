#include "ffpch.h"
#include "RenderCommand.h"
#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace Firefly
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
