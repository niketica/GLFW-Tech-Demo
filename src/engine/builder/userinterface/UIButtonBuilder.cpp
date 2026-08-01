#include "engine/builder/userinterface/UIButtonBuilder.h"

namespace niketica::builder
{

    UIButtonBuilder& UIButtonBuilder::withPosition(const glm::vec2& value)
    {
        this->position = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withSize(const glm::vec2& value)
    {
        this->size = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withPadding(const float value)
    {
        this->paddingTop = value;
        this->paddingBottom = value;
        this->paddingLeft = value;
        this->paddingRight = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withPaddingTop(const float value)
    {
        this->paddingTop = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withPaddingBottom(const float value)
    {
        this->paddingBottom = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withPaddingLeft(const float value)
    {
        this->paddingLeft = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withPaddingRight(const float value)
    {
        this->paddingRight = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withLayoutType(const niketica::component::UILayoutType value)
    {
        this->layoutType = value;
        return *this;
    }


    UIButtonBuilder& UIButtonBuilder::withAlignmentHorizontal(const niketica::component::AlignmentHorizontal value)
    {
        this->alignmentHorizontal = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withAlignmentVertical(const niketica::component::AlignmentVertical value)
    {
        this->alignmentVertical = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withFontSize(const float value)
    {
        this->fontSize = value;
        return *this;
    }

    UIButtonBuilder& UIButtonBuilder::withFontColor(const glm::vec4& value)
    {
        this->fontColor = value;
        return *this;
    }


    UIButtonBuilder& UIButtonBuilder::withTextLabel(const std::string& value)
    {
        this->text = value;
        return *this;
    }
    
    entt::entity UIButtonBuilder::createTextLabel(entt::entity parent)
    {
        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        auto entity = textLabelBuilder
            .withText(text)
            .withColor(fontColor)
            .withFontSize(fontSize)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .withAnchor({ alignmentHorizontal, alignmentVertical, { 0.0f, 0.0f } })
            .build();

        niketica::component::UIAlignment aligment;
        aligment.horizontal = alignmentHorizontal;
        aligment.vertical = alignmentVertical;

        niketica::component::ParentTransform parentTransform = { parent };
        registry->emplace<niketica::component::ParentTransform>(entity, parentTransform);
        registry->emplace<niketica::component::UIAlignment>(entity, aligment);

        return entity;
    }

    entt::entity UIButtonBuilder::build()
    {
        niketica::component::LocalTransform local;
        local.size =
        {
            size.x - paddingLeft * 2.0f,
            size.y,
            1.0f
        };

        niketica::component::UIAlignment aligment;
        aligment.horizontal = alignmentHorizontal;
        aligment.vertical = alignmentVertical;

        niketica::component::UILayout layout;
        layout.type = layoutType;
        layout.spacing = 0.0f;

        niketica::component::UISize uiSize;
        uiSize.widthMode = niketica::component::UISizeMode::PIXELS;
        uiSize.heightMode = niketica::component::UISizeMode::PIXELS;
        uiSize.width = size.x;
        uiSize.height = size.y;

        auto entity = registry->create();
        registry->emplace<niketica::component::UIButton>(entity);
        registry->emplace<niketica::component::NineSlice>(entity, niketica::config::NINE_SLICE_BUTTON_NORMAL);
        registry->emplace<niketica::component::Transform>(entity);
        registry->emplace<niketica::component::LocalTransform>(entity, local);
        registry->emplace<niketica::component::UIAlignment>(entity, aligment);
        registry->emplace<niketica::component::UIContentPadding>(entity);
        registry->emplace<niketica::component::UISpacing>(entity);
        registry->emplace<niketica::component::UILayout>(entity, layout);
        registry->emplace<niketica::component::UIAnchor>(entity, niketica::component::UIAnchor{ alignmentHorizontal, alignmentVertical, position });
        registry->emplace<niketica::component::UISize>(entity, uiSize);

        auto childEntity = createTextLabel(entity);        
        niketica::component::UIChildren children;
        children.children.emplace_back(childEntity);
        registry->emplace<niketica::component::UIChildren>(entity, children);

        return entity;
    }

}
