#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"
#include "engine/core/systems/ISystem.h"

namespace niketica::systems
{

    class ISystemContext
    {
    public:
        ISystemContext(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : registry(registry), engineServices(engineServices) {}
        virtual ~ISystemContext() = default;

        virtual void init() = 0;

        void input()
        {
            for (auto& sys : systems)
            {
                sys->input();
            }
        }

        void update(float dt)
        {
            for (auto& sys : systems)
            {
                sys->update(dt);
            }
        }

        void render()
        {
            for (auto& sys : systems)
            {
                sys->render();
            }
        }

    protected:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;

        std::vector<std::unique_ptr<ISystem>> systems;
        
    };

}
