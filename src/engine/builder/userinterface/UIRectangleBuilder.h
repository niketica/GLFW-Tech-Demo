#pragma once

#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"

namespace niketica::builder
{

    class UIRectangleBuilder
    {
    public:    
        UIRectangleBuilder(entt::registry* registry, niketica::engine::EngineServices* engineServices) : registry(registry), engineServices(engineServices) {}
        ~UIRectangleBuilder() = default;

        UIRectangleBuilder& withPosition(const glm::vec3& value);
        UIRectangleBuilder& withSize(const glm::vec2& value);
        UIRectangleBuilder& withFillColor(const glm::vec4& value);
        UIRectangleBuilder& withoutFill();
        UIRectangleBuilder& withScale(const float value);
        UIRectangleBuilder& withBorderColor(const glm::vec4& value);
        UIRectangleBuilder& withBorderThickness(const float value);
        UIRectangleBuilder& withAnchor(const niketica::component::UIAnchor& value);
        entt::entity build();
        
    private:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;

        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec2 size = { 100.0f, 100.0f };
        float scale = 1.0f;

        bool addFill = true;
        glm::vec4 fillColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        bool addBorder = false;
        float borderThickness = 1.0f;
        glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        niketica::component::UIAnchor anchor = { niketica::component::AlignmentHorizontal::LEFT, niketica::component::AlignmentVertical::TOP, { 0.0f, 0.0f } };
        
    };

}
