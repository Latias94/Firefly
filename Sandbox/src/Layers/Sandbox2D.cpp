#include "Sandbox2D.h"
#include <imgui.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
    FF_PROFILE_FUNCTION();

    m_CheckerboardTexture = Firefly::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    FF_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Firefly::Timestep ts)
{
    FF_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(ts);

    // Render
    {
        FF_PROFILE_SCOPE("Renderer Prep");
        Firefly::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Firefly::RenderCommand::Clear();
    }

    {
        FF_PROFILE_SCOPE("Renderer Draw");
        Firefly::Renderer2D::BeginScene(m_CameraController.GetCamera());
//        Firefly::Renderer2D::DrawRotatedQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, glm::radians(-45.0f),
//                                             {0.8f, 0.2f, 0.3f, 1.0f});
        Firefly::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Firefly::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.2f, 0.8f, 1.0f});
        // z=-0.1 cause opengl use right-hand coordinate system which its camera is face to -z.
        Firefly::Renderer2D::DrawQuad({-5.0f, -5.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture, 10.0f);
        Firefly::Renderer2D::DrawQuad({-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, m_CheckerboardTexture, 20.0f);
        Firefly::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    FF_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Firefly::Event& event)
{
    m_CameraController.OnEvent(event);
}
