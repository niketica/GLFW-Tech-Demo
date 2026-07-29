#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/config/Config.h"
#include "engine/builder/userinterface/UITextLabelBuilder.h"
#include "engine/builder/userinterface/UIButtonBuilder.h"

namespace niketica::builder
{
    enum class UIChildType
    {
        TEXT_LABEL,
        BUTTON
    };

    struct UIChild
    {
        UIChildType type;
        std::string text;
        glm::vec2 size;
    };

    class UIPanelBuilder
    {
    public:
        UIPanelBuilder(entt::registry* registry, niketica::engine::EngineServices* engineServices) : registry(registry), engineServices(engineServices) {}
        ~UIPanelBuilder() = default;

        UIPanelBuilder& withPosition(const glm::vec2& value);
        UIPanelBuilder& withSize(const glm::vec2& value);
        UIPanelBuilder& withPadding(const float value);
        UIPanelBuilder& withPaddingTop(const float value);
        UIPanelBuilder& withPaddingBottom(const float value);
        UIPanelBuilder& withPaddingLeft(const float value);
        UIPanelBuilder& withPaddingRight(const float value);
        UIPanelBuilder& withLayoutType(const niketica::component::UILayoutType value);

        UIPanelBuilder& withAlignmentHorizontal(const niketica::component::AlignmentHorizontal value);
        UIPanelBuilder& withAlignmentVertical(const niketica::component::AlignmentVertical value);
        UIPanelBuilder& withFontSize(const float value);
        UIPanelBuilder& withFontColor(const glm::vec4& value);

        UIPanelBuilder& addTextLabel(const std::string& value);
        UIPanelBuilder& addButton(const std::string& value, const glm::vec2& size);
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

        niketica::component::FontType fontType = niketica::component::FontType::OPEN_SANS_REGULAR;
        float fontSize = 14;
        glm::vec4 fontColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        niketica::component::AlignmentHorizontal alignmentHorizontal = niketica::component::AlignmentHorizontal::CENTER;
        niketica::component::AlignmentVertical alignmentVertical = niketica::component::AlignmentVertical::CENTER;
        niketica::component::UILayoutType layoutType = niketica::component::UILayoutType::VERTICAL;

        std::vector<UIChild> childElements;

        // Use hard coded values until there is a proper text measuring system
        float textHeight = 24.0f;
        float buttonHeight = 40.0f;
        float spacing = 8.0f;

        entt::entity createTextLabel(entt::entity parent, const std::string& text);
        entt::entity createButton(entt::entity panel, const std::string& text, const glm::vec2& size);

    };

}
