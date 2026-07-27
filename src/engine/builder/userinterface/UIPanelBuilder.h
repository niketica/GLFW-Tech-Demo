#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/config/Config.h"

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
        niketica::component::TextAlignmentVertical textAligmentVertical = niketica::component::TextAlignmentVertical::CENTER;
        niketica::component::TextAlignmentHorizontal textAligmentHorizontal = niketica::component::TextAlignmentHorizontal::CENTER;
    };

    class UIPanelBuilder
    {
    public:
        UIPanelBuilder(entt::registry* registry, niketica::engine::EngineServices* engineServices) : registry(registry), engineServices(engineServices) {}
        ~UIPanelBuilder() = default;

        UIPanelBuilder& withPosition(const glm::vec2& value);
        UIPanelBuilder& withSize(const glm::vec2& value);
        UIPanelBuilder& withPadding(const float value);
        UIPanelBuilder& withLayout(const niketica::component::UILayoutType value);

        UIPanelBuilder& addTextLabel(const std::string& value);
        UIPanelBuilder& addButton(const std::string& value, const glm::vec2& size);
        entt::entity build();
        
    private:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;

        glm::vec2 position;
        glm::vec2 size;
        float padding;
        niketica::component::UILayoutType layout;

        std::vector<UIChild> childElements;

        // Use hard coded values until there is a proper text measuring system
        float textHeight = 24.0f;
        float buttonHeight = 40.0f;
        float spacing = 8.0f;

        void createTextLabel(entt::entity panel, const std::string& text, float y);
        void createButton(entt::entity panel, const std::string& text, const glm::vec2& size, float y);

    };

}
