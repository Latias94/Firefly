#pragma once

#include "Firefly.h"

class ExampleLayer : public Firefly::Layer
{
public:
    ExampleLayer();
    ~ExampleLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Firefly::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Firefly::Event& event) override;

private:
    Firefly::ShaderLibrary m_ShaderLibrary;

    Firefly::Ref<Firefly::Shader>      m_Shader;
    Firefly::Ref<Firefly::VertexArray> m_VertexArray;

    Firefly::Ref<Firefly::Shader>      m_FlatColorShader;
    Firefly::Ref<Firefly::VertexArray> m_SquareVA;

    Firefly::Ref<Firefly::Texture2D> m_Texture;
    Firefly::Ref<Firefly::Texture2D> m_SecondTexture;

    Firefly::OrthographicCameraController m_CameraController;

    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};