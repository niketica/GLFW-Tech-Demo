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
    
    UIPanelBuilder& UIPanelBuilder::withAlignmentHorizontal(const niketica::component::AlignmentHorizontal value)
    {
        this->alignmentHorizontal = value;
        return *this;
    }
    
    UIPanelBuilder& UIPanelBuilder::withAlignmentVertical(const niketica::component::AlignmentVertical value)
    {
        this->alignmentVertical = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withFontSize(const float value)
    {
        this->fontSize = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withFontColor(const glm::vec4& value)
    {
        this->fontColor = value;
        return *this;
    }
    
    UIPanelBuilder& UIPanelBuilder::addTextLabel(const std::string& value)
    {
        UIChild child;
        child.type = UIChildType::TEXT_LABEL;
        child.text = value;
        childElements.emplace_back(child);

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
        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        auto textEntity = textLabelBuilder
            .withText(text)
            .withColor(fontColor)
            .withFontSize(fontSize)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .build();

        niketica::component::LocalTransform& local = registry->get<niketica::component::LocalTransform>(textEntity);
        local.position =
        {
            padding,
            y,
            0.0f
        };

        niketica::component::ParentTransform parentTransform = { parent };
        registry->emplace<niketica::component::ParentTransform>(textEntity, parentTransform);
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
