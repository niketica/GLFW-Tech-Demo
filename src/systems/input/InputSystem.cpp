#include "systems/input/InputSystem.h"

namespace niketica::systems
{

    void InputSystem::input()
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
    
}
