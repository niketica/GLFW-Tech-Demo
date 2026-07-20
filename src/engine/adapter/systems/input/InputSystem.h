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

        void input() override
        {
            auto viewInput = registry->view<niketica::component::InputComponent>();
            auto& input = viewInput.get<niketica::component::InputComponent>(viewInput.front());
            auto viewWindow = registry->view<niketica::component::Window>();
            const auto& window = viewWindow.get<niketica::component::Window>(viewWindow.front());

            auto* state = engineServices->getInputContext()->getInputState();
            input.mousePos = { state->mousePos.x, window.height - state->mousePos.y };
            input.mouseDelta = state->mouseDelta;
            input.scrollDelta = state->scrollDelta;

            mapBindings();

            for (const auto& [action, binding] : mapBindings())
            {
                const auto& key = binding.key;
                const auto& keyState = state->key(key);
                input.actions[action] = keyState;
            }
            engineServices->getInputContext()->clearState();
        }

        void update(float dt) override {}

        void render() override {}

    private:
        const std::unordered_map<niketica::component::Action, niketica::input::ActionBinding>& mapBindings() const
        {
            return engineServices->getInputContext()->getInputMap()->getBindings();
        }

    };

}
