#include "Sandbox2D.h"
#include <imgui.h>
#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{

}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Firefly::Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    // Render
    Firefly::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Firefly::RenderCommand::Clear();

    Firefly::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Firefly::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.2f, 0.3f, 1.0f});
    Firefly::Renderer2D::EndScene();

    // TODO: Add Shader::SetMat4, Shader::SetFloat4
//    std::dynamic_pointer_cast<Firefly::OpenGLShader>(m_FlatColorShader)->Bind();
//    std::dynamic_pointer_cast<Firefly::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color",
//                                                                                             m_SquareColor);

}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Firefly::Event& event)
{
    m_CameraController.OnEvent(event);
}
