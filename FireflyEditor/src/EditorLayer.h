#pragma once

#include "Firefly.h"
#include "Firefly/Scene/Scene.h"

namespace Firefly
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

    private:
        OrthographicCameraController m_CameraController;

        // temp
        Ref <VertexArray> m_SquareVA;

        Ref <Shader>      m_FlatColorShader;
        Ref <Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCameraEntity;

        bool m_PrimaryCamera = true;

        Ref <Texture2D>   m_CheckerboardTexture;

        bool      m_ViewportFocused = false;
        bool      m_ViewportHovered = false;
        glm::vec2 m_ViewportSize    = {0, 0};

        glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
    };
}