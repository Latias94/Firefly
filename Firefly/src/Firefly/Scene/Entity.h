#pragma once

#include "Scene.h"
#include <entt.hpp>

namespace Firefly
{
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&& ... args)
        {
            FF_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            FF_CORE_ASSERT(HasComponent<T>(), "Entity does not has component!");
            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
        }

        template<typename T>
        bool RemoveComponent()
        {
            FF_CORE_ASSERT(HasComponent<T>(), "Entity does not has component!");
            return m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }

        operator entt::entity() const { return m_EntityHandle; }

        operator uint32_t() const { return (uint32_t) m_EntityHandle; }

    private:
        entt::entity m_EntityHandle{entt::null}; // 4 bytes
        Scene* m_Scene = nullptr; // 8 bytes in 64x
    };
}
