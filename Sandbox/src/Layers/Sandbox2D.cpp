#include "Sandbox2D.h"
#include <imgui.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 24;

static const char* s_MapTiles =
                         "WWWWWWWWWWWWWWWWWWWWWWWW"
                         "WWWWWWWDDDDDDWWWWWWWWWWW"
                         "WWWWWDDDDDDDDDDDWWWWWWWW"
                         "WWWWDDDDDDDDDDDDDCDWWWWW"
                         "WWWDDDDDDDDDDDDDDDDDDWWW"
                         "WWDDDDWWWDDDDDDDDDDDDWWW"
                         "WDDDDDWWWDDDDDDDDDDDDDWW"
                         "WWDDDDDDDDDDDDDDDDDDDWWW"
                         "WWWWDDDDDDDDDDDDDDDDWWWW"
                         "WWWWWDDDDDDDDDDDDDDWWWWW"
                         "WWWWWWDDDDDDDDDDDDWWWWWW"
                         "WWWWWWWDDDDDDDDDDWWWWWWW"
                         "WWWWWWWWDDDDDDDDWWWWWWWW"
                         "WWWWWWWWWWWDDDDWWWWWWWWW"
                         "WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
    FF_PROFILE_FUNCTION();

    m_CheckerboardTexture = Firefly::Texture2D::Create("assets/textures/Checkerboard.png");
    m_SpriteSheet         = Firefly::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");
    m_SubTextureStair     = Firefly::SubTexture2D::CreateFromCoords(m_SpriteSheet, {7, 6}, {128, 128});
    m_SubTextureBarrel    = Firefly::SubTexture2D::CreateFromCoords(m_SpriteSheet, {8, 2}, {128, 128});
    m_SubTextureTree      = Firefly::SubTexture2D::CreateFromCoords(m_SpriteSheet, {2, 1}, {128, 128}, {1, 2});

    m_MapWidth  = s_MapWidth;
    m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

    s_TextureMap['D'] = Firefly::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6, 11}, {128, 128});
    s_TextureMap['W'] = Firefly::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11, 11}, {128, 128});

    m_CameraController.SetZoomLevel(5.0f);
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
    Firefly::Renderer2D::ResetStats();
    {
        FF_PROFILE_SCOPE("Renderer Prep");
        Firefly::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Firefly::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        FF_PROFILE_SCOPE("Renderer Draw");

        Firefly::Renderer2D::BeginScene(m_CameraController.GetCamera());

        for (uint32_t y = 0; y < m_MapHeight; ++y)
        {
            for (uint32_t x = 0; x < m_MapWidth; ++x)
            {
                char tileType = s_MapTiles[x + y * m_MapWidth];

                Firefly::Ref<Firefly::SubTexture2D> texture;
                if (s_TextureMap.find(tileType) != s_TextureMap.end())
                {
                    texture = s_TextureMap[tileType];
                } else
                {
                    texture = m_SubTextureStair;
                }
                Firefly::Renderer2D::DrawQuad({x - m_MapWidth / 2.0f, m_MapHeight / 2.0f - y, 0.5f},
                                              {1.0f, 1.0f}, texture);
            }
        }

        Firefly::Renderer2D::DrawQuad({-1.0f, 0.0f, 0.1f}, {1.0f, 1.0f}, m_SubTextureStair, 1.0f);
        Firefly::Renderer2D::DrawQuad({0.0f, 0.0f, 0.1f}, {1.0f, 1.0f}, m_SubTextureBarrel, 1.0f);
        Firefly::Renderer2D::DrawQuad({1.0f, 0.0f, 0.1f}, {1.0f, 2.0f}, m_SubTextureTree, 1.0f);
        Firefly::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    FF_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = Firefly::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Firefly::Event& event)
{
    m_CameraController.OnEvent(event);
}
