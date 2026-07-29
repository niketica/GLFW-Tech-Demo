#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/config/Config.h"
#include "engine/core/EngineServices.h"
#include "engine/builder/userinterface/UITextLabelBuilder.h"

namespace niketica::builder
{

    class UIButtonBuilder
    {
    public:    
        UIButtonBuilder(entt::registry* registry, niketica::engine::EngineServices* engineServices) : registry(registry), engineServices(engineServices) {}
        ~UIButtonBuilder() = default;

        UIButtonBuilder& withPosition(const glm::vec2& value);
        UIButtonBuilder& withSize(const glm::vec2& value);
        UIButtonBuilder& withPadding(const float value);
        UIButtonBuilder& withPaddingTop(const float value);
        UIButtonBuilder& withPaddingBottom(const float value);
        UIButtonBuilder& withPaddingLeft(const float value);
        UIButtonBuilder& withPaddingRight(const float value);
        UIButtonBuilder& withLayoutType(const niketica::component::UILayoutType value);

        UIButtonBuilder& withAlignmentHorizontal(const niketica::component::AlignmentHorizontal value);
        UIButtonBuilder& withAlignmentVertical(const niketica::component::AlignmentVertical value);
        UIButtonBuilder& withFontSize(const float value);
        UIButtonBuilder& withFontColor(const glm::vec4& value);

        UIButtonBuilder& withTextLabel(const std::string& value);
        entt::entity build();
        
    private:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;

        glm::vec2 position;
        glm::vec2 size;
        float paddingTop = 0.0f;
        float paddingBottom = 0.0f;
        float paddingLeft = 0.0f;
        float paddingRight = 0.0f;

        std::string text = "";
        niketica::component::FontType fontType = niketica::component::FontType::OPEN_SANS_REGULAR;
        float fontSize = 14;
        glm::vec4 fontColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        niketica::component::AlignmentHorizontal alignmentHorizontal = niketica::component::AlignmentHorizontal::CENTER;
        niketica::component::AlignmentVertical alignmentVertical = niketica::component::AlignmentVertical::CENTER;
        niketica::component::UILayoutType layoutType = niketica::component::UILayoutType::VERTICAL;

        // Use hard coded values until there is a proper text measuring system
        float textHeight = 24.0f;
        float buttonHeight = 40.0f;
        float spacing = 8.0f;

        entt::entity createTextLabel(entt::entity parent);
        
    };

}