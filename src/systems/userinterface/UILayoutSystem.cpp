#include "systems/userinterface/UILayoutSystem.h"

namespace niketica::systems
{

    void UILayoutSystem::update(float dt)
    {
        auto viewRenderSettings = registry->view<niketica::component::RenderSettings>();
        const auto& renderSettings = viewRenderSettings.get<niketica::component::RenderSettings>(viewRenderSettings.front());

        if (isGlobalLayoutDirty())
        {
            updateUIText(renderSettings);
            updateUINineSlice(renderSettings);
            updateAllContainers(renderSettings);
        }

        updateDirtyContainers(renderSettings);
    }

    void UILayoutSystem::updateAllContainers(const niketica::component::RenderSettings& renderSettings)
    {
        auto viewContainers = registry->view<niketica::component::UIChildren>(entt::exclude<component::ParentTransform>);
        for (auto container : viewContainers)
        {
            registry->remove<niketica::component::UIContainerLayoutDirty>(container);
            updatePositionContainer(container, renderSettings);
            updateLayoutContainer(container);
        }
    }

    void UILayoutSystem::updateDirtyContainers(const niketica::component::RenderSettings& renderSettings)
    {
        auto viewLayoutDirty = registry->view<niketica::component::UIContainerLayoutDirty>();
        for (auto container : viewLayoutDirty)
        {
            registry->remove<niketica::component::UIContainerLayoutDirty>(container);
            updatePositionContainer(container, renderSettings);
            updateLayoutContainer(container);        
        }
    }

    bool UILayoutSystem::isGlobalLayoutDirty() const
    {
        bool isDirty = false;
        auto viewLayoutDirty = registry->view<niketica::component::UIGlobalLayoutDirty>();
        for (auto entity : viewLayoutDirty)
        {
            registry->remove<niketica::component::UIGlobalLayoutDirty>(entity);
            isDirty = true;
        }
        return isDirty;
    }

    void UILayoutSystem::updatePositionContainer(entt::entity container, const niketica::component::RenderSettings& renderSettings)
    {
        const auto& alignment = registry->get<niketica::component::UIAlignment>(container);
        auto& transform = registry->get<niketica::component::LocalTransform>(container);
        auto& position = transform.position;
        const auto& size = transform.size;

        float virtualWidth = renderSettings.uiReferenceResolution.x;
        float virtualHeight = renderSettings.uiReferenceResolution.y;

        switch (alignment.horizontal)
        {
        case niketica::component::AlignmentHorizontal::CENTER:
            position.x = (virtualWidth * 0.5f) - (size.x * 0.5f);
            break;
        case niketica::component::AlignmentHorizontal::LEFT:
            position.x = 0.0f;
            break;
        case niketica::component::AlignmentHorizontal::RIGHT:
            position.x = virtualWidth - size.x;
            break;
        }

        switch (alignment.vertical)
        {
        case niketica::component::AlignmentVertical::CENTER:
            position.y = (virtualHeight * 0.5f) - (size.y * 0.5f);
            break;
        case niketica::component::AlignmentVertical::TOP:
            position.y = virtualHeight - size.y;
            break;
        case niketica::component::AlignmentVertical::BOTTOM:
            position.y = 0.0f;
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
        float availableWidth = containerSizeX - padding.left - padding.right;
        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            const auto& alignment = registry->get<niketica::component::UIAlignment>(child);
            childLocal.position.z = containerPosZ + 0.01f;
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

    void UILayoutSystem::updateLayoutHorizontal(entt::entity container)
    {
        const auto& children = registry->get<niketica::component::UIChildren>(container);
        const auto& transform = registry->get<niketica::component::LocalTransform>(container);
        const auto& size = transform.size;
        const auto& padding = registry->get<niketica::component::UIContentPadding>(container);
        const auto& spacing = registry->get<niketica::component::UISpacing>(container);
        const auto& containerAlignment = registry->get<niketica::component::UIAlignment>(container);

        const float totalWidth = measureTotalWidth(children, spacing.spacing);
        float cursorX = computeStartX(size.x, totalWidth, padding, containerAlignment.horizontal);
        placeChildrenHorizontally(children, spacing.spacing, cursorX);
        alignChildrenVertically(children, transform.position.z, size.y, padding);
    }

    float UILayoutSystem::measureTotalWidth(const niketica::component::UIChildren& children, const float spacing)
    {
        float totalWidth = 0.0f;

        for (auto child : children.children)
        {
            const auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            totalWidth += childLocal.size.x;
        }

        if (!children.children.empty())
        {
            totalWidth += spacing * (children.children.size() - 1);
        }

        return totalWidth;
    }

    float UILayoutSystem::computeStartX(const float containerSizeX, const float totalWidth, const niketica::component::UIContentPadding padding, const niketica::component::AlignmentHorizontal alignment) const
    {
        float availableWidth = containerSizeX - padding.left - padding.right;
        float startX = 0.0f;

        switch (alignment)
        {
        case niketica::component::AlignmentHorizontal::LEFT:
            startX = padding.left;
            break;
        case niketica::component::AlignmentHorizontal::CENTER:
            startX = padding.left + (availableWidth - totalWidth) * 0.5f;
            break;
        case niketica::component::AlignmentHorizontal::RIGHT:
            startX = containerSizeX - padding.right - totalWidth;
            break;
        }

        return startX;
    }
    
    void UILayoutSystem::placeChildrenHorizontally(const niketica::component::UIChildren& children, const float spacing, float cursorX)
    {
        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            childLocal.position.x = cursorX;
            cursorX += childLocal.size.x;
            cursorX += spacing;
        }
    }
    
    void UILayoutSystem::alignChildrenVertically(const niketica::component::UIChildren& children, const float containerPosZ, const float containerSizeY, const niketica::component::UIContentPadding padding)
    {
        float availableHeight = containerSizeY - padding.top - padding.bottom;
        for (auto child : children.children)
        {
            auto& childLocal = registry->get<niketica::component::LocalTransform>(child);
            const auto& alignment = registry->get<niketica::component::UIAlignment>(child);
            childLocal.position.z = containerPosZ + 0.01f;
            switch(alignment.vertical)
            {
            case niketica::component::AlignmentVertical::TOP:
                childLocal.position.y = containerSizeY - padding.top - childLocal.size.y;
                break;
            case niketica::component::AlignmentVertical::CENTER:
                childLocal.position.y = padding.bottom + (availableHeight - childLocal.size.y) * 0.5f;
                break;
            case niketica::component::AlignmentVertical::BOTTOM:
                childLocal.position.y = padding.bottom;
                break;
            }
        }
    }

    void UILayoutSystem::updateUIText(const niketica::component::RenderSettings& renderSettings)
    {
        auto viewUIStuff = registry->view<niketica::component::UIAnchor, niketica::component::UISize, niketica::component::LocalTransform, niketica::component::Text>();
        for (auto entity : viewUIStuff)
        {
            const auto& anchor = viewUIStuff.get<niketica::component::UIAnchor>(entity);
            const auto& size = viewUIStuff.get<niketica::component::UISize>(entity);
            auto& local = viewUIStuff.get<niketica::component::LocalTransform>(entity);
            auto& text = viewUIStuff.get<niketica::component::Text>(entity);

            const auto& uiReferenceResolution = renderSettings.uiReferenceResolution;
            float scaleX = uiReferenceResolution.x / niketica::config::ORIGINAL_WIDTH;
            float scaleY = uiReferenceResolution.y / niketica::config::ORIGINAL_HEIGHT;

            auto calculatedPosition = calculatePosition({ uiReferenceResolution.x, uiReferenceResolution.y }, { scaleX, scaleY }, anchor);
            auto calculatedSize = calculateSize({ scaleX, scaleY }, size);

            text.fontSize = (int)calculatedSize.y;
            local.position = { calculatedPosition.x, calculatedPosition.y, 0.0f };
            local.size = { calculatedSize.x, calculatedSize.y, 0.0f };
        }
    }

    void UILayoutSystem::updateUINineSlice(const niketica::component::RenderSettings& renderSettings)
    {
        auto viewNineSlice = registry->view<niketica::component::UIAnchor, niketica::component::UISize, niketica::component::LocalTransform, niketica::component::NineSlice>();
        for (auto entity : viewNineSlice)
        {
            const auto& anchor = viewNineSlice.get<niketica::component::UIAnchor>(entity);
            const auto& size = viewNineSlice.get<niketica::component::UISize>(entity);
            auto& local = viewNineSlice.get<niketica::component::LocalTransform>(entity);

            const auto& uiReferenceResolution = renderSettings.uiReferenceResolution;
            float scaleX = uiReferenceResolution.x / niketica::config::ORIGINAL_WIDTH;
            float scaleY = uiReferenceResolution.y / niketica::config::ORIGINAL_HEIGHT;

            auto calculatedPosition = calculatePosition({ uiReferenceResolution.x, uiReferenceResolution.y }, { scaleX, scaleY }, anchor);
            auto calculatedSize = calculateSize({ scaleX, scaleY }, size);

            local.position = { calculatedPosition.x, calculatedPosition.y, 0.0f };
            local.size = { calculatedSize.x, calculatedSize.y, 0.0f };
        }
    }

    glm::vec2 UILayoutSystem::calculatePosition(const glm::vec2& resolution, const glm::vec2& scale, const niketica::component::UIAnchor& anchor) const
    {
        float offsetX = anchor.offset.x * scale.x;
        float offsetY = anchor.offset.y * scale.y;

        float posX = 0.0f;
        float posY = 0.0f;

        switch (anchor.horizontal)
        {
        case niketica::component::AlignmentHorizontal::LEFT:
            posX = 0.0f + offsetX;
            break;
        case niketica::component::AlignmentHorizontal::RIGHT:
            posX = resolution.x + offsetX;
            break;
        case niketica::component::AlignmentHorizontal::CENTER:
            posX = (resolution.x * 0.5f) + offsetX;
            break;
        }

        switch (anchor.vertical)
        {
        case niketica::component::AlignmentVertical::TOP:
            posY = resolution.y + offsetY;
            break;
        case niketica::component::AlignmentVertical::BOTTOM:
            posY = 0.0f + offsetY;
            break;
        case niketica::component::AlignmentVertical::CENTER:
            posY = (resolution.y * 0.5f) + offsetY;
            break;
        }

        return { posX, posY };
    }

    glm::vec2 UILayoutSystem::calculateSize(const glm::vec2& scale, const niketica::component::UISize& size) const
    {
        float sizeX = 0.0f;
        float sizeY = 0.0f;
        switch (size.widthMode)
        {
        case niketica::component::UISizeMode::PIXELS:
            sizeX = size.width * scale.x;
            break;
        case niketica::component::UISizeMode::PERCENT:
            sizeX = size.width * scale.x; // TODO calc percentage of window height?
            break;
        }
        
        switch (size.heightMode)
        {
        case niketica::component::UISizeMode::PIXELS:
            sizeY = size.height * scale.y;
            break;
        case niketica::component::UISizeMode::PERCENT:
            sizeY = size.height * scale.y; // TODO calc percentage of window height?
            break;
        }

        return { sizeX, sizeY };
    }
    
}
