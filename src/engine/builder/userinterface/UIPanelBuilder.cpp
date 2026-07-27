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
        this->paddingTop = value;
        this->paddingBottom = value;
        this->paddingLeft = value;
        this->paddingRight = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withPaddingTop(const float value)
    {
        this->paddingTop = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withPaddingBottom(const float value)
    {
        this->paddingBottom = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withPaddingLeft(const float value)
    {
        this->paddingLeft = value;
        return *this;
    }

    UIPanelBuilder& UIPanelBuilder::withPaddingRight(const float value)
    {
        this->paddingRight = value;
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
        auto panelEntity = registry->create();

        niketica::component::Transform transformUI;
        transformUI.position = { position.x, position.y, 1.0f };
        transformUI.size = { size.x, size.y, 1.0f };

        niketica::component::UIContentPadding paddingComponent;
        paddingComponent.top = paddingTop;
        paddingComponent.bottom = paddingBottom;
        paddingComponent.left = paddingLeft;
        paddingComponent.right = paddingRight;

        niketica::component::UISpacing spacingComponent = { spacing };

        niketica::component::UIAlignment aligment;
        aligment.horizontal = alignmentHorizontal;
        aligment.vertical = alignmentVertical;

        float cursorY = size.y - paddingTop;

        niketica::component::UIChildren children;
        for (const auto& child : childElements)
        {
            switch (child.type)
            {
                case UIChildType::TEXT_LABEL:
                {
                    cursorY -= textHeight;
                    auto childEntity = createTextLabel(panelEntity, child.text, cursorY);
                    children.children.emplace_back(childEntity);
                    cursorY -= spacing;
                    break;
                }

                case UIChildType::BUTTON:
                {
                    cursorY -= child.size.y;
                    auto childEntity = createButton(panelEntity, child.text, child.size, cursorY);
                    children.children.emplace_back(childEntity);
                    cursorY -= spacing;
                    break;
                }
            }
        }

        registry->emplace<niketica::component::UIPanel>(panelEntity);
        registry->emplace<niketica::component::NineSlice>(panelEntity, niketica::config::NINE_SLICE_PANEL);
        registry->emplace<niketica::component::Transform>(panelEntity, transformUI);
        registry->emplace<niketica::component::UIChildren>(panelEntity, children);
        registry->emplace<niketica::component::UIContentPadding>(panelEntity, paddingComponent);
        registry->emplace<niketica::component::UISpacing>(panelEntity, spacingComponent);
        registry->emplace<niketica::component::UIAlignment>(panelEntity, aligment);

        return panelEntity;
    }
    
    entt::entity UIPanelBuilder::createTextLabel(entt::entity parent, const std::string& text, float y)
    {
        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        auto entity = textLabelBuilder
            .withText(text)
            .withColor(fontColor)
            .withFontSize(fontSize)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .build();

        niketica::component::LocalTransform& local = registry->get<niketica::component::LocalTransform>(entity);
        local.position =
        {
            paddingLeft,
            y,
            0.0f
        };

        niketica::component::ParentTransform parentTransform = { parent };
        registry->emplace<niketica::component::ParentTransform>(entity, parentTransform);

        return entity;
    }

    entt::entity UIPanelBuilder::createButton(entt::entity parent, const std::string& text, const glm::vec2& buttonSize, float y)
    {
        niketica::component::ParentTransform parentTransform = { parent };
        niketica::component::LocalTransform local;
        local.position =
        {
            paddingLeft,
            y,
            0.1f
        };
        local.size =
        {
            buttonSize.x - paddingLeft * 2.0f,
            buttonSize.y,
            1.0f
        };

        auto entity = registry->create();
        registry->emplace<niketica::component::UIButton>(entity);
        registry->emplace<niketica::component::NineSlice>(entity, niketica::config::NINE_SLICE_BUTTON_NORMAL);
        registry->emplace<niketica::component::Transform>(entity);
        registry->emplace<niketica::component::LocalTransform>(entity, local);
        registry->emplace<niketica::component::ParentTransform>(entity, parentTransform);

        createTextLabel(entity, text, paddingTop);
        return entity;
    }

}
