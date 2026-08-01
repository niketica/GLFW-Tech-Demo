#include "engine/builder/userinterface/UITextLabelBuilder.h"

namespace niketica::builder
{

    UITextLabelBuilder& UITextLabelBuilder::withText(const std::string& value)
    {
        this->text = value;
        return *this;
    }

    UITextLabelBuilder& UITextLabelBuilder::withPosition(const glm::vec2& value)
    {
        this->position = value;
        return *this;
    }
    
    UITextLabelBuilder& UITextLabelBuilder::withFontType(const niketica::component::FontType value)
    {
        this->fontType = value;
        return *this;
    }
    
    UITextLabelBuilder& UITextLabelBuilder::withFontSize(const float value)
    {
        this->fontSize = value;
        return *this;
    }

    UITextLabelBuilder& UITextLabelBuilder::withColor(const glm::vec4& value)
    {
        this->color = value;
        return *this;
    }

    UITextLabelBuilder& UITextLabelBuilder::withScale(const float value)
    {
        this->scale = value;
        return *this;
    }
    
    entt::entity UITextLabelBuilder::build()
    {
        niketica::component::Text textComponent;
        textComponent.value = text;
        textComponent.fontSize = (int)fontSize;
        textComponent.color = color;
        textComponent.scale = scale;
        textComponent.fontType = fontType;

        niketica::component::LocalTransform local;
        local.position = { position.x, position.y, 0.0f };
        local.size = { ((float)text.length() * (fontSize * 0.6)), fontSize, 1.0f };

        niketica::component::UISize uiSize;
        uiSize.widthMode = niketica::component::UISizeMode::PIXELS;
        uiSize.heightMode = niketica::component::UISizeMode::PIXELS;
        uiSize.width = ((float)text.length() * (fontSize * 0.6));
        uiSize.height = fontSize;

        niketica::component::UIAnchor anchor;
        anchor.horizontal = niketica::component::AlignmentHorizontal::LEFT;
        anchor.vertical = niketica::component::AlignmentVertical::TOP;
        anchor.offset = { 100.0f, -100.0f };

        auto entity = registry->create();
        registry->emplace<niketica::component::Text>(entity, textComponent);
        registry->emplace<niketica::component::LocalTransform>(entity, local);
        registry->emplace<niketica::component::Transform>(entity);
        registry->emplace<niketica::component::UISize>(entity, uiSize);
        registry->emplace<niketica::component::UIAnchor>(entity, anchor);

        return entity;
    }
    
}
