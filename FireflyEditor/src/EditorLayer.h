#pragma once

#include "Firefly.h"

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
        Ref <Texture2D>   m_CheckerboardTexture;

        struct ProfileResult
        {
            const char* Name;
            float Time;
        };

        glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
    };
}