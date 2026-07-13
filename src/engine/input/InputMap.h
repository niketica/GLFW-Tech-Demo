#pragma once

#include <unordered_map>

#include "component/Input.h"

namespace niketica::engine
{
    struct ActionBinding
    {
        int key;
    };

    class InputMap
    {
    public:
        void bind(niketica::component::Action action, int key)
        {
            bindings[action] = { key };
        }

        const ActionBinding* get(niketica::component::Action action) const
        {
            auto it = bindings.find(action);
            return it != bindings.end() ? &it->second : nullptr;
        }

        const std::unordered_map<niketica::component::Action, ActionBinding>& getBindings() const
        {
            return bindings;
        }

    private:
        std::unordered_map<niketica::component::Action, ActionBinding> bindings;
    };

}