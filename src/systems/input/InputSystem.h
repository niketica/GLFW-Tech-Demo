#pragma once

#include <entt/entt.hpp>

#include "component/Input.h"
#include "component/Window.h"
#include "engine/core/input/InputState.h"
#include "engine/core/input/InputMap.h"

namespace niketica::systems
{
    class InputSystem
    {
    public:
        InputSystem(const niketica::input::InputState* state, const niketica::input::InputMap* map, entt::registry* registry)
            : state(state), map(map), registry(registry) {}

        void update()
        {
            auto viewInput = registry->view<niketica::component::InputComponent>();
            auto& input = viewInput.get<niketica::component::InputComponent>(viewInput.front());
            auto viewWindow = registry->view<niketica::component::Window>();
            const auto& window = viewWindow.get<niketica::component::Window>(viewWindow.front());

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
        }

    private:
        const niketica::input::InputState* state;
        const niketica::input::InputMap* map;

        entt::registry* registry;

        const std::unordered_map<niketica::component::Action, niketica::input::ActionBinding>& mapBindings() const
        {
            return map->getBindings();
        }
    };

}
