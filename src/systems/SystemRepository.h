#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "systems/input/InputSystem.h"

namespace niketica::systems
{
    class SystemRepository
    {
        public:
            SystemRepository(entt::registry* registry, const niketica::engine::InputState& state, const niketica::engine::InputMap& map);
            ~SystemRepository() = default;

            void reset();

            InputSystem* getInputSystem() const { return inputSystem.get(); }
        
        private:
            entt::registry* registry;
            const niketica::engine::InputState& state;
            const niketica::engine::InputMap& map;

            std::unique_ptr<InputSystem> inputSystem;

            void initSystems();
    };

}
