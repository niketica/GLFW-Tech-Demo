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
        const auto& layout = registry->get<niketica::component::UILayout>(container);
        switch(layout.type)
        {
            case niketica::component::UILayoutType::VERTICAL:
                updateLayoutVertical(container);
                break;

            case niketica::component::UILayoutType::HORIZONTAL:
                updateLayoutHorizontal(container);
                break;
        }

        const auto& children = registry->get<niketica::component::UIChildren>(container);
        for (auto child : children.children)
        {
            if (registry->any_of<niketica::component::UIChildren>(child))
            {
                updateLayoutContainer(child);
            }
        }
    }
        
    void UILayoutSystem::updateLayoutVertical(entt::entity container)
    {
        const auto& children = registry->get<niketica::component::UIChildren>(container);
        const auto& transform = registry->get<niketica::component::LocalTransform>(container);
        const auto& size = transform.size;
        const auto& padding = registry->get<niketica::component::UIContentPadding>(container);
        const auto& spacing = registry->get<niketica::component::UISpacing>(container);
        const auto& containerAlignment = registry->get<niketica::component::UIAlignment>(container);

        const float totalHeight = measureTotalHeight(children, spacing.spacing);
        float cursorY = computeStartY(size.y, totalHeight, padding, containerAlignment.vertical);
        placeChildrenVertically(children, spacing.spacing, cursorY);
        alignChildrenHorizontally(children, transform.position.z, size.x, padding);
    }

    void UILayoutSystem::updateLayoutHorizontal(entt::entity container)
    {

    }

    float UILayoutSystem::measureTotalHeight(const niketica::component::UIChildren& children, const float spacing)
    {
        float totalHeight = 0.0f;

        for (auto child : children.children)
        {
            const auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            totalHeight += childLocal.size.y;
        }

        if (!children.children.empty())
        {
            totalHeight += spacing * (children.children.size() - 1);
        }

        return totalHeight;
    }

    float UILayoutSystem::computeStartY(const float containerSizeY, const float totalHeight, const niketica::component::UIContentPadding padding, const niketica::component::AlignmentVertical alignment) const
    {
        float availableHeight = containerSizeY - padding.top - padding.bottom;
        float startY = 0.0f;

        switch (alignment)
        {
        case niketica::component::AlignmentVertical::TOP:
            startY = containerSizeY - padding.top;
            break;
        case niketica::component::AlignmentVertical::CENTER:
            startY = padding.bottom + (availableHeight + totalHeight) * 0.5f;
            break;
        case niketica::component::AlignmentVertical::BOTTOM:
            startY = padding.bottom + totalHeight;
            break;
        }

        return startY;
    }

    void UILayoutSystem::placeChildrenVertically(const niketica::component::UIChildren& children, const float spacing, float cursorY)
    {
        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            cursorY -= childLocal.size.y;
            childLocal.position.y = cursorY;
            cursorY -= spacing;
        }
    }

    void UILayoutSystem::alignChildrenHorizontally(const niketica::component::UIChildren& children, const float containerPosZ, const float containerSizeX, const niketica::component::UIContentPadding padding)
    {
        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            const auto& alignment = registry->get<niketica::component::UIAlignment>(child);
            childLocal.position.z = containerPosZ + 0.01f;
            float availableWidth = containerSizeX - padding.left - padding.right;
            switch (alignment.horizontal)
            {
            case niketica::component::AlignmentHorizontal::LEFT:
                childLocal.position.x = padding.left;
                break;

            case niketica::component::AlignmentHorizontal::CENTER:
                childLocal.position.x = padding.left + (availableWidth - childLocal.size.x) * 0.5f;
                break;

            case niketica::component::AlignmentHorizontal::RIGHT:
                childLocal.position.x = containerSizeX - padding.right - childLocal.size.x;
                break;
            }
        }
    }
    
}
