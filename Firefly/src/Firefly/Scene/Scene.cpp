#include "ffpch.h"
#include "Firefly/Scene/Scene.h"
#include "Firefly/Scene/Entity.h"
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

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::OnUpdate(Timestep ts)
    {
        // Render 2D
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;
        {
            auto      group = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity: group)
            {
                auto[transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform.Transform;
                    break;
                }
            }
        }
        if (mainCamera)
        {
            Renderer2D::BeginScene(*mainCamera, *cameraTransform);

            auto group = m_Registry.group<TransformComponent>(entt::get < SpriteRendererComponent > );

            for (auto entity: group)
            {
                auto[transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform, sprite.Color);
            }
            Renderer2D::EndScene();
        }
    }
}