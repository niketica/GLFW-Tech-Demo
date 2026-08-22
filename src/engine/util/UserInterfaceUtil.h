#pragma once

#include <vector>
#include <entt/entt.hpp>

#include "component/Components.h"

namespace niketica::util
{

    static inline void setFocusables(entt::registry* registry, const std::vector<entt::entity>& newFocusables)
    {
        auto viewFocus = registry->view<niketica::component::UIFocus>();
        if (!viewFocus.empty())
        {
            auto& focusables = viewFocus.get<niketica::component::UIFocus>(viewFocus.front()).focusables;
            focusables.clear();
            focusables = newFocusables;
        }
    }

}
