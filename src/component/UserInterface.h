#pragma once

#include <string>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/TextureHandle.h"
#include "component/Input.h"

namespace niketica::component
{

    /* #################
       ### RENDERING ###
       ################# */
    // (maybe move this to Sprite.h or a new Rendering header)
    struct NineSlice
    {
        const char* texture;

        glm::vec2 spriteOffset; // top-left in pixels
        glm::vec2 spriteSize;   // width / height in pixels

        // pixel borders
        float left;
        float right;
        float top;
        float bottom;
    };

    /* ###################
       ### UI BEHAVIOR ###
       ################### */
    struct UIPanel {};
    struct UIButton {};
    struct UILabel {};

    struct UIDraggable
    {
        bool dragging = false;
    };

    /* ##############################
       ### LAYOUT AND POSITIONING ###
       ############################## */
    enum class TextAlignmentHorizontal
    {
        LEFT,
        RIGHT,
        CENTER
    };

    enum class TextAlignmentVertical
    {
        TOP,
        BOTTOM,
        CENTER
    };

    struct UIAnchor
    {
        TextAlignmentHorizontal horizontal = TextAlignmentHorizontal::LEFT;
        TextAlignmentVertical vertical = TextAlignmentVertical::TOP;
    };

    // Padding is the space between an element's content and its border, creating inner space that keeps the content away from the edges.
    struct UIPadding
    {
        float left = 0;
        float right = 0;
        float top = 0;
        float bottom = 0;
    };

    // Margin is the space outside an element’s border that separates it from surrounding elements, ensuring proper spacing within the layout.
    struct UIMargin
    {
        float left = 0;
        float right = 0;
        float top = 0;
        float bottom = 0;
    };

    struct UIPreferredSize
    {
        glm::vec2 size;
    };

    enum class UILayoutType
    {
        Absolute,
        Vertical,
        Horizontal
    };

    struct UILayout
    {
        UILayoutType type = UILayoutType::Vertical;
        float spacing = 8.0f;
    };

    struct UIAlignment
    {
        TextAlignmentHorizontal horizontal = TextAlignmentHorizontal::LEFT;
        TextAlignmentVertical vertical = TextAlignmentVertical::TOP;
    };

    struct UIContentPadding
    {
        float left;
        float right;
        float top;
        float bottom;
    };

    struct UIText
    {
        TextAlignmentHorizontal horizontal;
        TextAlignmentVertical vertical;
    };

}
