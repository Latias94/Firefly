#include "ffpch.h"
#include "Firefly/Scene/Scene.h"
#include "Firefly/Scene/Components.h"
#include "Firefly/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace Firefly
{
    static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {}

    Scene::Scene()
    {
        #if ENTT_EXAMPLE_CODE
        entt::entity entity = m_Registry.create();
        m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
        m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

        TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
        auto view = m_Registry.view<TransformComponent>();

        for (auto entity: view)
        {
            // all transform in the scene
            TransformComponent& transform = view.get<TransformComponent>(entity);
        }
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity: group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform, sprite.Color);
        }
        #endif
    }

    Scene::~Scene()
    {

    }

    entt::entity Scene::CreateEntity()
    {
        return m_Registry.create();
    }

    void Scene::OnUpdate(Timestep ts)
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity: group)
        {
            auto[transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            Renderer2D::DrawQuad(transform, sprite.Color);
        }
    }
}