#pragma once

#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"

namespace niketica::builder
{

    class UITextLabelBuilder
    {
    public:    
        UITextLabelBuilder(entt::registry* registry, niketica::engine::EngineServices* engineServices) : registry(registry), engineServices(engineServices) {}
        ~UITextLabelBuilder() = default;

        UITextLabelBuilder& withText(const std::string& value);
        UITextLabelBuilder& withPosition(const glm::vec2& value);
        UITextLabelBuilder& withFontType(const niketica::component::FontType value);
        UITextLabelBuilder& withFontSize(const float value);
        UITextLabelBuilder& withColor(const glm::vec4& value);
        UITextLabelBuilder& withScale(const float value);
        UITextLabelBuilder& withAnchor(const niketica::component::UIAnchor& value);
        entt::entity build();
        
    private:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;

        std::string text = "";
        glm::vec2 position = { 0.0f, 0.0f };
        niketica::component::FontType fontType = niketica::component::FontType::OPEN_SANS_REGULAR;
        float fontSize = 14;
        glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        float scale = 1.0f;
        niketica::component::UIAnchor anchor = { niketica::component::AlignmentHorizontal::LEFT, niketica::component::AlignmentVertical::TOP, { 0.0f, 0.0f } };
        
    };

}
