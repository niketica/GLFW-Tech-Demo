#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{

    class UINavigationSystem : public ISystem
    {
    public:
        UINavigationSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}
        ~UINavigationSystem() = default;

        void input() override {}
        void update(float dt) override;
        void render() override {};

    };

}
