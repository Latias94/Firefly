#include "ffpch.h"
#include "Renderer.h"

namespace Firefly
{
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        // Take scene parameter, take shader, texture, camera, lighting and etc. store in the current scene
        // and wait for uploading.
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray,
                          const glm::mat4& transform)
    {
        shader->Bind();
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        shader->UploadUniformMat4("u_Transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}