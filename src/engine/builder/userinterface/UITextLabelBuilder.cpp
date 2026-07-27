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
    
    UITextLabelBuilder& UITextLabelBuilder::withAlignmentHorizontal(const niketica::component::TextAlignmentHorizontal value)
    {
        this->alignmentHorizontal = value;
        return *this;
    }
    
    UITextLabelBuilder& UITextLabelBuilder::withAlignmentVertical(const niketica::component::TextAlignmentVertical value)
    {
        this->alignmentVertical = value;
        return *this;
    }
    
    entt::entity UITextLabelBuilder::build()
    {
        auto entity = registry->create();

        niketica::component::Text textComponent;
        textComponent.value = text;
        textComponent.fontSize = (int)fontSize;
        textComponent.color = color;
        textComponent.scale = scale;
        textComponent.fontType = fontType;

        niketica::component::LocalTransform local;
        local.position = { position.x, position.y, 0.0f };

        auto textEntity = registry->create();
        registry->emplace<niketica::component::Text>(textEntity, textComponent);
        registry->emplace<niketica::component::LocalTransform>(textEntity, local);
        registry->emplace<niketica::component::Transform>(textEntity);

        return entity;
    }
    
}
