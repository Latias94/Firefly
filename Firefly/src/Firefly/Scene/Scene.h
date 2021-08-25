#pragma once

#include <entt.hpp>

namespace Firefly
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

    private:
        entt::registry m_Registry;
    };
}
