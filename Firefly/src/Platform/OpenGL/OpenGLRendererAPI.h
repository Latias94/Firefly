#include "ffpch.h"
#include <Firefly/Renderer/RendererAPI.h>

namespace Firefly
{
    class OpenGLRendererAPI : public RendererAPI
    {
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };
}