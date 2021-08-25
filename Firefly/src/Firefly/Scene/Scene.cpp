#include "ffpch.h"
#include "Scene.h"
#include <glm/glm.hpp>

namespace Firefly
{
    static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {}

    Scene::Scene()
    {
        struct MeshComponent
        {
        };
        struct TransformComponent
        {
            glm::mat4 Transform;
            TransformComponent() = default;
            TransformComponent(const TransformComponent&) = default;

            TransformComponent(const glm::mat4 transform) : Transform(transform) {}

            operator const glm::mat4&() { return Transform; }
        };

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
        auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);

        for (auto entity: group)
        {
            auto tuple = group.get<TransformComponent, MeshComponent>(entity);
        }
    }

    Scene::~Scene()
    {

    }
}