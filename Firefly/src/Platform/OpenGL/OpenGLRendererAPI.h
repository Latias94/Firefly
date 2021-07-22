#include "ffpch.h"
#include <Firefly/Renderer/RendererAPI.h>

namespace Firefly
{
    class OpenGLRendererAPI : public RendererAPI
    {
        void Init() override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void DrawIndexed(const Ref <VertexArray>& vertexArray) override;
    };
}