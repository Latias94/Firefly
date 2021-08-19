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

    // temp
    Firefly::Ref<Firefly::VertexArray> m_SquareVA;

    Firefly::Ref<Firefly::Shader> m_FlatColorShader;

    Firefly::Ref<Firefly::Texture2D>    m_CheckerboardTexture;
    Firefly::Ref<Firefly::Texture2D>    m_SpriteSheet;
    Firefly::Ref<Firefly::SubTexture2D> m_SubTextureStair, m_SubTextureBarrel, m_SubTextureTree;

    struct ProfileResult
    {
        const char* Name;
        float Time;
    };

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};

    std::unordered_map<char, Firefly::Ref<Firefly::SubTexture2D>> s_TextureMap;

    uint32_t m_MapWidth, m_MapHeight;

};