#include "ffpch.h"
#include "Renderer.h"

namespace Firefly
{
    void Renderer::BeginScene()
    {
        // Take scene parameter, take shader, texture, camera, lighting and etc. store in the current scene
        // and wait for uploading.
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}