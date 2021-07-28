#pragma once

#include "Firefly.h"

class Sandbox2D : public Firefly::Layer
{
public:
    Sandbox2D();
    ~Sandbox2D() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Firefly::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Firefly::Event& event) override;

private:
    Firefly::OrthographicCameraController m_CameraController;

    Firefly::Ref<Firefly::VertexArray> m_SquareVA;

    Firefly::Ref<Firefly::Shader> m_FlatColorShader;
    glm::vec4                     m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};


};