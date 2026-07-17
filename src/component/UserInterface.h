#pragma once

#include <string>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/TextureHandle.h"
#include "component/Input.h"

namespace niketica::component
{
    enum class UIState
    {
        NORMAL,
        HOVERED,
        FOCUSED,
        DISABLED
    };
    
    struct UIPanel {};

    struct UIParent
    {
        entt::entity parent = entt::null;
    };

    struct UIContext
    {
        entt::entity ctx = entt::null;
    };

    struct UIChildren
    {
        std::vector<entt::entity> children;
    };

    struct UIButton
    {
        bool selectable = true;
    };

    struct UIToggle
    {
        bool value = false;
    };

    struct UIRadioButton
    {
        int groupId = 0;
        bool selected = false;
    };

    enum class UITextAlign
    {
        LEFT,
        CENTER
    };

    struct UILabel
    {
        std::string text;
        int fontSize = 24;

        UITextAlign align = UITextAlign::CENTER;

        glm::vec3 normalColor = { 1,1,1 };
        glm::vec3 selectedColor = { 1,1,0 };
    };

    struct UIFocus
    {
        int index = 0;
    };

    struct UINineSlice
    {
        TextureHandle texture;

        glm::vec2 spriteOffset; // top-left in pixels
        glm::vec2 spriteSize;   // width / height in pixels

        // pixel borders
        float left;
        float right;
        float top;
        float bottom;
    };

    struct UIActive {};

    struct UIButtonActivated {};

    struct UISubPanel
    {
        entt::entity parent;
    };

    struct UISpace {};

    enum class UILayoutDirection
    {
        VERTICAL,
        HORIZONTAL
    };

    struct UILayout
    {
        UILayoutDirection direction = UILayoutDirection::VERTICAL;

        float padding = 16.0f;
        float spacing = 8.0f;

        glm::vec2 minButtonSize = { 280.0f, 32.0f };
        glm::vec2 minToggleSize = { 49.0f, 43.0f };
    };

    struct UIPanelStyle
    {
        UINineSlice normal;
    };

    struct UIButtonStyle
    {
        UINineSlice normal;
        UINineSlice focused;
        UINineSlice pressed;
        UINineSlice disabled;
    };

    struct UIToggleStyle
    {
        UINineSlice on;
        UINineSlice off;
    };

    enum class UIPanelAnchorType
    {
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT,
        CENTER,
        CENTER_LEFT,
        CENTER_RIGHT,
        CENTER_TOP,
        CENTER_BOTTOM
    };

    struct UIPanelAnchor
    {
        UIPanelAnchorType type;
        glm::vec2 offset;
    };

    struct UICreateButton {};

    struct UICreateToggle {};

    struct UICreateRadio {};

    struct UICreateLabel
    {
        std::string text;
        int fontSize = 24;
        component::UITextAlign align = component::UITextAlign::CENTER;
    };

    struct UIUpdatePosition {};

    struct UICreatePanel
    {
        UILayout layout;
        glm::vec3 position;
        UIPanelAnchorType anchorType = UIPanelAnchorType::CENTER;
    };

    struct UIDestroyPanel {};

    struct UIHandleNagigation {};

}
