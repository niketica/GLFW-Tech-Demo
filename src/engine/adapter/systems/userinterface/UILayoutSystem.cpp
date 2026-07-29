#include "engine/adapter/systems/userinterface/UILayoutSystem.h"

namespace niketica::systems
{

    void UILayoutSystem::update(float dt)
    {
        // TODO add component UILayoutDirty so that calculations are not done every frame.
        
        auto viewWindow = registry->view<niketica::component::Window>();
        const auto& window = registry->get<niketica::component::Window>(viewWindow.front());

        auto viewRootContainers = registry->view<niketica::component::UIPanel>();
        for (auto container : viewRootContainers)
        {
            updatePositionContainer(container, window);
            updateLayoutContainer(container);
        }
    }

    void UILayoutSystem::updatePositionContainer(entt::entity container, const niketica::component::Window& window)
    {
        const auto& alignment = registry->get<niketica::component::UIAlignment>(container);
        auto& transform = registry->get<niketica::component::LocalTransform>(container);
        auto& position = transform.position;
        const auto& size = transform.size;

        float windowWidth = (float)window.width;
        float windowHeight = (float)window.height;

        switch (alignment.horizontal)
        {
        case niketica::component::AlignmentHorizontal::CENTER:
            position.x = (windowWidth * 0.5f) - (size.x * 0.5f);
            break;
        case niketica::component::AlignmentHorizontal::LEFT:
            position.x = 0.0f;
            break;
        case niketica::component::AlignmentHorizontal::RIGHT:
            position.x = windowWidth - size.x;
            break;
        }

        switch (alignment.vertical)
        {
        case niketica::component::AlignmentVertical::CENTER:
            position.y = (windowHeight * 0.5f) - (size.y * 0.5f);
            break;
        case niketica::component::AlignmentVertical::TOP:
            position.y = 0.0f;
            break;
        case niketica::component::AlignmentVertical::BOTTOM:
            position.y = windowHeight - size.y;
            break;
        }
    }

    void UILayoutSystem::updateLayoutContainer(entt::entity container)
    {
        updateLayoutChildElements(container);
        const auto& children = registry->get<niketica::component::UIChildren>(container);
        for (auto child : children.children)
        {
            if (registry->any_of<niketica::component::UIChildren>(child))
            {
                updateLayoutContainer(child);
            }
        }
    }

    void UILayoutSystem::updateLayoutChildElements(entt::entity container)
    {
        const auto& children = registry->get<niketica::component::UIChildren>(container);
        const auto& transform = registry->get<niketica::component::LocalTransform>(container);
        const auto& size = transform.size;
        const auto& padding = registry->get<niketica::component::UIContentPadding>(container);
        const auto& spacing = registry->get<niketica::component::UISpacing>(container);
        const auto& containerAlignment = registry->get<niketica::component::UIAlignment>(container);

        float totalHeight = 0.0f;

        for (auto child : children.children)
        {
            const auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            totalHeight += childLocal.size.y;
        }

        if (!children.children.empty())
        {
            totalHeight += spacing.spacing * (children.children.size() - 1);
        }

        float availableHeight = size.y - padding.top - padding.bottom;
        float cursorY = 0.0f;

        switch (containerAlignment.vertical)
        {
        case niketica::component::AlignmentVertical::TOP:
            cursorY = size.y - padding.top;
            break;

        case niketica::component::AlignmentVertical::CENTER:
            cursorY = padding.bottom +
                    (availableHeight + totalHeight) * 0.5f;
            break;

        case niketica::component::AlignmentVertical::BOTTOM:
            cursorY = padding.bottom + totalHeight;
            break;
        }

        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            cursorY -= childLocal.size.y;
            childLocal.position.y = cursorY;
            cursorY -= spacing.spacing;
        }

        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            const auto& alignment = registry->get<niketica::component::UIAlignment>(child);
            childLocal.position.z = transform.position.z + 0.01f;
            float availableWidth = size.x - padding.left - padding.right;
            switch (alignment.horizontal)
            {
            case niketica::component::AlignmentHorizontal::LEFT:
                childLocal.position.x = padding.left;
                break;

            case niketica::component::AlignmentHorizontal::CENTER:
                childLocal.position.x = padding.left + (availableWidth - childLocal.size.x) * 0.5f;
                break;

            case niketica::component::AlignmentHorizontal::RIGHT:
                childLocal.position.x = size.x - padding.right - childLocal.size.x;
                break;
            }
        }
    }
    
}
