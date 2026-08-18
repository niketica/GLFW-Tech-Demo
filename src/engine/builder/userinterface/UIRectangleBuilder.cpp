#include "engine/builder/userinterface/UIRectangleBuilder.h"

namespace niketica::builder
{
        UIRectangleBuilder& UIRectangleBuilder::withPosition(const glm::vec3& value)
        {
            this->position = value;
            return *this;
        }

        UIRectangleBuilder& UIRectangleBuilder::withSize(const glm::vec2& value)
        {
            this->size = value;
            return *this;
        }

        UIRectangleBuilder& UIRectangleBuilder::withFillColor(const glm::vec4& value)
        {
            this->fillColor = value;
            return *this;
        }

        UIRectangleBuilder& UIRectangleBuilder::withoutFill()
        {
            this->addFill = false;
            return *this;
        }
        
        UIRectangleBuilder& UIRectangleBuilder::withScale(const float value)
        {
            this->scale = value;
            return *this;
        }
        
        UIRectangleBuilder& UIRectangleBuilder::withBorderColor(const glm::vec4& value)
        {
            this->addBorder = true;
            this->borderColor = value;
            return *this;
        }
        
        UIRectangleBuilder& UIRectangleBuilder::withBorderThickness(const float value)
        {
            this->addBorder = true;
            this->borderThickness = value;
            return *this;
        }

        UIRectangleBuilder& UIRectangleBuilder::withAnchor(const niketica::component::UIAnchor& value)
        {
            this->anchor = value;
            return *this;
        }
        
        entt::entity UIRectangleBuilder::build()
        {
            auto transform = niketica::component::Transform
            {
                position,
                { size.x, size.y, 0.0f },
                { scale, scale, scale }
            };
            
            auto entity = registry->create();
            registry->emplace<niketica::component::Rectangle>(entity);
            registry->emplace<niketica::component::Transform>(entity, transform);

            if (addFill)
            {
                auto fillColorCmpnt = niketica::component::FillColor{ fillColor };
                registry->emplace<niketica::component::FillColor>(entity, fillColorCmpnt);
            }

            if (addBorder)
            {
                auto borderColorCmpnt = niketica::component::BorderColor{ borderColor };
                auto borderThicknessCmpnt = niketica::component::BorderThickness{ borderThickness };
                registry->emplace<niketica::component::BorderColor>(entity, borderColorCmpnt);
                registry->emplace<niketica::component::BorderThickness>(entity, borderThicknessCmpnt);
            }

            return entity;
        }
}
