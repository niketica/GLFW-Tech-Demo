#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"

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

        niketica::component::NineSlice nineSlicePanel = {
            engineServices->getRenderContext()->getTextureLoader()->acquire("textures/ui/ui_sheet.dds"),
            { 814.0f, 0.0f },   // offset
            { 206.0f, 209.0f },    // size
            25, 25, 28, 25 };
        niketica::component::NineSlice nineSliceButtonNormal = {
            engineServices->getRenderContext()->getTextureLoader()->acquire("textures/ui/ui_sheet.dds"),
            { 602.0f, 0.0f },   // offset
            { 212.0f, 74.0f },    // size
            24, 24, 24, 24 };
        niketica::component::NineSlice nineSliceButtonFocused = {
            engineServices->getRenderContext()->getTextureLoader()->acquire("textures/ui/ui_sheet.dds"),
            { 602.0f, 74.0f },   // offset
            { 212.0f, 74.0f },    // size
            24, 24, 24, 24 };
        niketica::component::NineSlice nineSliceButtonPressed = {
            engineServices->getRenderContext()->getTextureLoader()->acquire("textures/ui/ui_sheet.dds"),
            { 602.0f, 148.0f },   // offset
            { 212.0f, 74.0f },    // size
            24, 24, 24, 24 };

        // Use hard coded values until there is a proper text measuring system
        float textHeight = 24.0f;
        float buttonHeight = 40.0f;
        float spacing = 8.0f;

        void createTextLabel(entt::entity panel, const std::string& text, float y);
        void createButton(entt::entity panel, const std::string& text, const glm::vec2& size, float y);

    };

}
