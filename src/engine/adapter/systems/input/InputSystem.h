#pragma once

#include <entt/entt.hpp>

#include "component/Input.h"
#include "component/Window.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{
    class InputSystem : public ISystem
    {
    public:
        InputSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input();
        void update(float dt) override {}
        void render() override {}

    private:
        const std::unordered_map<niketica::component::Action, niketica::input::ActionBinding>& mapBindings() const
        {
            return engineServices->getInputContext()->getInputMap()->getBindings();
        }

    };

}
