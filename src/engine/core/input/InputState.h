#pragma once

#include <unordered_map>
#include <glm/vec2.hpp>

#include "component/Input.h"

namespace niketica::input
{

    class InputState
    {
    public:
        void clearState()
        {
            for (auto& [_, state] : keys)
            {
                state.pressed = false;
                state.released = false;
            }

            mouseDelta = { 0.0f, 0.0f };
            scrollDelta = 0.0f;
        }

        niketica::component::ButtonState& key(int key)
        {
            return keys[key];
        }

        const niketica::component::ButtonState& key(int key) const
        {
            static niketica::component::ButtonState empty{};
            auto it = keys.find(key);
            return it != keys.end() ? it->second : empty;
        }

        glm::vec2 mousePos{ 0.0f };
        glm::vec2 mouseDelta{ 0.0f };
        float scrollDelta{ 0.0f };

    private:
        std::unordered_map<int, niketica::component::ButtonState> keys;
    };

}
