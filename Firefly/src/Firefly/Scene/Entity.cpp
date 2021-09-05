#include "Entity.h"

namespace Firefly
{
    Entity::Entity(entt::entity handle, Scene* scene)
            : m_EntityHandle(handle), m_Scene(scene)
    {


    }
}