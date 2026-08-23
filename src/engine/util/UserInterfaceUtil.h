#pragma once

#include <vector>
#include <entt/entt.hpp>

#include "component/Components.h"

namespace niketica::util::ui
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

    static inline void updateFocusedVisualsContainer(entt::registry* registry, const bool focus, entt::entity entity)
    {
        if (registry->all_of<niketica::component::UINormalColor, niketica::component::UIHighlightColor>(entity))
        {
            if (focus)
            {
                const auto& highlightColor = registry->get<niketica::component::UIHighlightColor>(entity).color;
                
                if (registry->all_of<niketica::component::Color>(entity))
                {
                    auto& color = registry->get<niketica::component::Color>(entity).value;
                    color = highlightColor;
                }
                if (registry->all_of<niketica::component::Text>(entity))
                {
                    auto& text = registry->get<niketica::component::Text>(entity);
                    text.color = highlightColor;
                }
            }
            else
            {
                const auto& normalColor = registry->get<niketica::component::UINormalColor>(entity).color;
                
                if (registry->all_of<niketica::component::Color>(entity))
                {
                    auto& color = registry->get<niketica::component::Color>(entity).value;
                    color = normalColor;
                }
                if (registry->all_of<niketica::component::Text>(entity))
                {
                    auto& text = registry->get<niketica::component::Text>(entity);
                    text.color = normalColor;
                }
            }
        }

        if (registry->all_of<niketica::component::UIChildren>(entity))
        {
            const auto& children = registry->get<niketica::component::UIChildren>(entity).children;
            for (auto child : children)
            {
                updateFocusedVisualsContainer(registry, focus, child);
            }
        }
    }

    static inline void updateTextSize(entt::registry* registry, entt::entity entity)
    {
        const auto& text = registry->get<niketica::component::Text>(entity);

        auto width = (float)((float)text.value.length() * (text.fontSize * 0.6));
        auto height = (float)text.fontSize;

        auto& local = registry->get<niketica::component::LocalTransform>(entity).size;
        local.x = width;
        local.y = height;

        auto& uiSize = registry->get<niketica::component::UISize>(entity);
        uiSize.widthMode = niketica::component::UISizeMode::PIXELS;
        uiSize.heightMode = niketica::component::UISizeMode::PIXELS;
        uiSize.width = width;
        uiSize.height = height;
    }

}
