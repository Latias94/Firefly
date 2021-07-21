#include "ffpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

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

    void Renderer::Submit(Ref<Shader>& shader, const Ref<VertexArray>& vertexArray,
                          const glm::mat4& transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection",
                                                                           m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}