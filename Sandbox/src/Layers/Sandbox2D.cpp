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
    m_SquareVA = Firefly::VertexArray::Create();

    float squareVertices[3 * 4] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
    };

    Firefly::Ref<Firefly::VertexBuffer> squareVB;
    squareVB.reset(Firefly::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

    Firefly::BufferLayout squareVBLayout = {
            {Firefly::ShaderDataType::Float3, "a_Position"},
    };

    squareVB->SetLayout(squareVBLayout);
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

    Firefly::Ref<Firefly::IndexBuffer> squareIB;
    squareIB.reset(Firefly::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(squareIB);

    m_FlatColorShader = Firefly::Shader::Create("assets/shaders/FlatColor.glsl");
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

    Firefly::Renderer::BeginScene(m_CameraController.GetCamera());


    std::dynamic_pointer_cast<Firefly::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Firefly::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color",
                                                                                             m_SquareColor);
    Firefly::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Firefly::Renderer::EndScene();

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
