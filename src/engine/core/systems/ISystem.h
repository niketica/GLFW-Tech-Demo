#pragma once

#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"

namespace niketica::systems
{

    class ISystem
    {
    public:
        ISystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : registry(registry), engineServices(engineServices) {}
        virtual ~ISystem() = default;

        virtual void input() = 0;
        virtual void update(float dt) = 0;
        virtual void render() = 0;

    protected:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;
    };

}
