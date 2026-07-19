#pragma once

#include <iostream>
#include <unordered_map>

#include "component/Input.h"

namespace niketica::input
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
            std::cout << "INFO::InputMap::bind - START" << std::endl;
            bindings[action] = { key };
            std::cout << "INFO::InputMap::bind - END" << std::endl;
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