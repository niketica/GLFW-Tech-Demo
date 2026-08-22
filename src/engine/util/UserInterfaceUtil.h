#pragma once

#include <vector>
#include <entt/entt.hpp>

#include "component/Components.h"

namespace niketica::util
{

    static inline void setFocusables(entt::registry* registry, const std::vector<entt::entity>& newFocusables)
    {
        auto viewFocus = registry->view<niketica::component::UIFocus>();
        auto& focusables = viewFocus.get<niketica::component::UIFocus>(viewFocus.front()).focusables;
        focusables.clear();
        focusables = newFocusables;
    }

    static inline void clearFocusables(entt::registry* registry)
    {
        auto viewFocus = registry->view<niketica::component::UIFocus>();
        auto& focusables = viewFocus.get<niketica::component::UIFocus>(viewFocus.front()).focusables;
        focusables.clear();
    }

    static inline void addFocusable(entt::registry* registry, entt::entity newFocusable)
    {
        auto viewFocus = registry->view<niketica::component::UIFocus>();
        auto& focusables = viewFocus.get<niketica::component::UIFocus>(viewFocus.front()).focusables;
        focusables.emplace_back(newFocusable);
    }

}
