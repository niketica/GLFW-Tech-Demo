#include "engine/adapter/systems/userinterface/UILayoutSystem.h"

namespace niketica::systems
{

    void UILayoutSystem::update(float dt)
    {
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
        const auto& alignment = registry->get<niketica::component::UIAnchor>(container);
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

        float cursorY = size.y - padding.top;

        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            const auto& alignment = registry->get<niketica::component::UIAnchor>(child);

            cursorY -= childLocal.size.y;
            childLocal.position.y = cursorY;
            cursorY -= spacing.spacing;

            childLocal.position.z = transform.position.z + 0.01f;
            
            switch (alignment.horizontal)
            {
            case niketica::component::AlignmentHorizontal::CENTER:
            {
                float centerPanel = size.x * 0.5f;
                float halfSizeChild = childLocal.size.x * 0.5f;
                childLocal.position.x = centerPanel - halfSizeChild;
            }
            break;
            case niketica::component::AlignmentHorizontal::LEFT:
            {
                childLocal.position.x = padding.left;
            }
            break;
            case niketica::component::AlignmentHorizontal::RIGHT:
            {
                childLocal.position.x = size.x - padding.right - childLocal.size.x;                    
            }
            break;
            }
            
        }
    }
    
}
