#include "engine/factory/userinterface/BasePanelFactory.h"

namespace niketica::factory::ui
{

    entt::entity BasePanelFactory::createContainer
    (
        const glm::vec2& size,
        const float spacing,
        const niketica::component::UILayoutType layout
    )
    {
        auto entity = registry->create();
        makeContainer(entity, size, spacing, layout);
        return entity;
    }
    
    void BasePanelFactory::makeContainer
    (
        entt::entity entity,
        const glm::vec2& size,
        const float spacing,
        const niketica::component::UILayoutType layout
    )
    {
        niketica::component::UIAlignment aligment;
        aligment.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        aligment.vertical = niketica::component::AlignmentVertical::CENTER;

        niketica::component::UILayout layoutCmpnt;
        layoutCmpnt.type = layout;
        niketica::component::UIPadding padding;
        niketica::component::UIContentPadding contentPadding;
        niketica::component::UISpacing spacingCmpnt;
        spacingCmpnt.spacing = spacing;
        niketica::component::UISize uiSize;
        uiSize.width = size.x;
        uiSize.height = size.y;

        registry->emplace<niketica::component::UIAlignment>(entity, aligment);
        registry->emplace<niketica::component::UILayout>(entity, layoutCmpnt);
        registry->emplace<niketica::component::UIPadding>(entity, padding);
        registry->emplace<niketica::component::UIContentPadding>(entity, contentPadding);
        registry->emplace<niketica::component::UISpacing>(entity, spacingCmpnt);
        registry->emplace<niketica::component::UISize>(entity, uiSize);
        registry->emplace<niketica::component::UIChildren>(entity);
    }

    void BasePanelFactory::addChildToContainer(entt::entity container, entt::entity child)
    {
        if (!registry->all_of<niketica::component::Transform>(child))
        {
            registry->emplace<niketica::component::Transform>(child);
        }
        if (!registry->all_of<niketica::component::LocalTransform>(child))
        {
            registry->emplace<niketica::component::LocalTransform>(child);
        }

        niketica::component::ParentTransform parentTransform = { container };
        registry->emplace<niketica::component::ParentTransform>(child, parentTransform);

        if (registry->any_of<niketica::component::UIChildren>(container))
        {
            auto& children = registry->get<niketica::component::UIChildren>(container).children;
            children.emplace_back(child);
        }
        else
        {
            niketica::component::UIChildren containerChildren;
            containerChildren.children.emplace_back(child);
            registry->emplace<niketica::component::UIChildren>(container, containerChildren);
        }
    }

    entt::entity BasePanelFactory::createContainerRect
    (
        const char* fillColor,
        const char* borderColor,
        const glm::vec2 size,
        float spacing
    )
    {
        auto fillColorVec4 = niketica::util::color::colorFromHexRGB(fillColor);
        auto borderColorVec4 = niketica::util::color::colorFromHexRGB(borderColor);
        niketica::builder::UIRectangleBuilder rectBuilder = { registry, engineServices };
        auto entity = rectBuilder
            .withSize(size)
            .withFillColor(fillColorVec4)
            .withBorderColor(borderColorVec4)
            .withBorderThickness(4.0f)
            .build();

        makeContainer(entity, size, spacing, niketica::component::UILayoutType::VERTICAL);

        return entity;
    }

    entt::entity BasePanelFactory::createButton(const char* text, const glm::vec2& size, const float padding)
    {
        auto textLabel = createTextLabel(text, 20.0f);
        auto button = createContainerRect("152E18", "09140A", size);
        setPadding(button, padding);
        addChildToContainer(button, textLabel);
        return button;
    }

    entt::entity BasePanelFactory::createTextLabel(const char* text, float fontSize)
    {
        auto textNormalColor = niketica::util::color::colorFromHexRGB("000000");
        auto textHighlightColor = niketica::util::color::colorFromHexRGB("FFFFFF");

        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        auto entity = textLabelBuilder
            .withText(text)
            .withFontSize(fontSize)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .withColor(textNormalColor)
            .withPosition(glm::vec2{ 0.0f, 0.0f })
            .build();

        registry->emplace<niketica::component::UINormalColor>(entity, niketica::component::UINormalColor{ textNormalColor });
        registry->emplace<niketica::component::UIHighlightColor>(entity, niketica::component::UIHighlightColor{ textHighlightColor });
            
        niketica::component::UIAlignment aligment;
        aligment.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        aligment.vertical = niketica::component::AlignmentVertical::CENTER;
        registry->emplace<niketica::component::UIAlignment>(entity, aligment);

        return entity;
    }

    void BasePanelFactory::setPadding(entt::entity entity, const float value)
    {
        auto& padding = registry->get_or_emplace<niketica::component::UIContentPadding>(entity);
        padding.top = value;
        padding.bottom = value;
        padding.left = value;
        padding.right = value;
    }

}
