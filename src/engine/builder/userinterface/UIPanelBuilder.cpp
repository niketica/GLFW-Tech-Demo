#include "engine/builder/userinterface/UIPanelBuilder.h"

namespace niketica::builder
{

    UIPanelBuilder& UIPanelBuilder::withPosition(const glm::vec2& value)
    {
        this->position = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withSize(const glm::vec2& value)
    {
        this->size = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withPadding(const float value)
    {
        this->padding = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withLayout(const niketica::component::UILayoutType value)
    {
        this->layout = value;
        return *this;
    }
    
    UIPanelBuilder& UIPanelBuilder::addTextLabel(const std::string& value)
    {
        childElements.emplace_back(UIChild{ UIChildType::TEXT_LABEL, value });
        return *this;
    }
    
    UIPanelBuilder& UIPanelBuilder::addButton(const std::string& value, const glm::vec2& size)
    {
        childElements.emplace_back(UIChild{ UIChildType::BUTTON, value, size });
        return *this;        
    }
    
    entt::entity UIPanelBuilder::build()
    {
        niketica::component::Transform transformUI;
        transformUI.position = { position.x, position.y, 1.0f };
        transformUI.size = { size.x, size.y, 1.0f };

        auto panelEntity = registry->create();
        registry->emplace<niketica::component::NineSlice>(panelEntity, niketica::config::NINE_SLICE_PANEL);
        registry->emplace<niketica::component::Transform>(panelEntity, transformUI);

        float cursorY = size.y - padding;

        for (const auto& child : childElements)
        {
            switch (child.type)
            {
                case UIChildType::TEXT_LABEL:
                {
                    cursorY -= textHeight;
                    createTextLabel(panelEntity, child.text, cursorY);
                    cursorY -= spacing;
                    break;
                }

                case UIChildType::BUTTON:
                {
                    cursorY -= child.size.y;
                    createButton(panelEntity, child.text, child.size, cursorY);
                    cursorY -= spacing;
                    break;
                }
            }
        }

        return panelEntity;
    }
    
    void UIPanelBuilder::createTextLabel(entt::entity parent, const std::string& text, float y)
    {
        niketica::component::Text textComponent;
        textComponent.fontSize = 20;
        textComponent.value = text;
        textComponent.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
        textComponent.scale = 1.0f;
        textComponent.fontType = niketica::component::FontType::COURIER_PRIME_CODE;

        niketica::component::ParentTransform parentTransform = { parent };
        niketica::component::LocalTransform local;
        local.position =
        {
            padding,
            y,
            0.0f
        };

        auto entity = registry->create();
        registry->emplace<niketica::component::Text>(entity, textComponent);
        registry->emplace<niketica::component::Transform>(entity);
        registry->emplace<niketica::component::LocalTransform>(entity, local);
        registry->emplace<niketica::component::ParentTransform>(entity, parentTransform);
    }

    void UIPanelBuilder::createButton(entt::entity parent, const std::string& text, const glm::vec2& buttonSize, float y)
    {
        niketica::component::ParentTransform parentTransform = { parent };
        niketica::component::LocalTransform local;
        local.position =
        {
            padding,
            y,
            0.1f
        };
        local.size =
        {
            buttonSize.x - padding * 2.0f,
            buttonSize.y,
            1.0f
        };

        auto button = registry->create();
        registry->emplace<niketica::component::UIButton>(button);
        registry->emplace<niketica::component::NineSlice>(button, niketica::config::NINE_SLICE_BUTTON_NORMAL);
        registry->emplace<niketica::component::Transform>(button);
        registry->emplace<niketica::component::LocalTransform>(button, local);
        registry->emplace<niketica::component::ParentTransform>(button, parentTransform);

        createTextLabel(button, text, padding);
    }

}
