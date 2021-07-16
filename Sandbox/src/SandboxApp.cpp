#include <Firefly.h>

#include <memory>
#include "imgui.h"

class ExampleLayer : public Firefly::Layer
{
public:
    ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        m_VertexArray.reset(Firefly::VertexArray::Create());

        float vertices[3 * 7] = {
                -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
        };

        std::shared_ptr<Firefly::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Firefly::VertexBuffer::Create(vertices, sizeof(vertices)));
        Firefly::BufferLayout layout = {
                {Firefly::ShaderDataType::Float3, "a_Position"},
                {Firefly::ShaderDataType::Float4, "a_Color"},
        };

        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};

        std::shared_ptr<Firefly::IndexBuffer> indexBuffer;
        indexBuffer.reset(Firefly::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        m_SquareVA.reset(Firefly::VertexArray::Create());

        float squareVertices[3 * 4] = {
                -0.75f, -0.75f, 0.0f,
                0.75f, -0.75f, 0.0f,
                0.75f, 0.75f, 0.0f,
                -0.75f, 0.75f, 0.0f,
        };

        std::shared_ptr<Firefly::VertexBuffer> squareVB;
        squareVB.reset(Firefly::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        Firefly::BufferLayout squareVBLayout = {
                {Firefly::ShaderDataType::Float3, "a_Position"},
        };

        squareVB->SetLayout(squareVBLayout);
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};

        std::shared_ptr<Firefly::IndexBuffer> squareIB;
        squareIB.reset(Firefly::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        // location 0 is the AttribPointer.
        std::string vertexSrc   = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;

            uniform mat4 u_ViewProjection;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";
        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";
        m_Shader = std::make_shared<Firefly::Shader>(vertexSrc, fragmentSrc);

        std::string blueShaderVertexSrc2   = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position;
            uniform mat4 u_ViewProjection;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
            }
        )";
        std::string blueShaderFragmentSrc2 = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader = std::make_shared<Firefly::Shader>(blueShaderVertexSrc2, blueShaderFragmentSrc2);

    }

    void OnUpdate(Firefly::Timestep ts) override
    {

        FF_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());
        if (Firefly::Input::IsKeyPressed(FF_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Firefly::Input::IsKeyPressed(FF_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;
        if (Firefly::Input::IsKeyPressed(FF_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
        else if (Firefly::Input::IsKeyPressed(FF_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;

        if (Firefly::Input::IsKeyPressed(FF_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (Firefly::Input::IsKeyPressed(FF_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        Firefly::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Firefly::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Firefly::Renderer::BeginScene(m_Camera);

        Firefly::Renderer::Submit(m_BlueShader, m_SquareVA);
        Firefly::Renderer::Submit(m_Shader, m_VertexArray);

        Firefly::Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {
    }

    void OnEvent(Firefly::Event& event) override
    {
    }

private:
    std::shared_ptr<Firefly::Shader>      m_Shader;
    std::shared_ptr<Firefly::VertexArray> m_VertexArray;

    std::shared_ptr<Firefly::Shader>      m_BlueShader;
    std::shared_ptr<Firefly::VertexArray> m_SquareVA;

    Firefly::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition;
    float     m_CameraMoveSpeed     = 5.0f;
    float     m_CameraRotation      = 0.0f;
    float     m_CameraRotationSpeed = 180.0f;
};

class Sandbox : public Firefly::Application
{
public :
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {
    }
};

Firefly::Application* Firefly::CreateApplication()
{
    return new Sandbox();
}
