#pragma once

#include <string>
#include <glm/glm.hpp>

#include "component/Components.h"

namespace niketica::config
{
    inline constexpr const char* UI_TEXTURE = "textures/ui/ui_sheet.dds";    

    inline constexpr niketica::component::NineSlice NINE_SLICE_PANEL = {
        UI_TEXTURE,
        glm::vec2{ 814.0f, 0.0f },   // offset
        glm::vec2{ 206.0f, 209.0f },    // size
        25, 25, 28, 25 };

    inline constexpr niketica::component::NineSlice NINE_SLICE_BUTTON_NORMAL = {
        UI_TEXTURE,
        glm::vec2{ 602.0f, 0.0f },   // offset
        glm::vec2{ 212.0f, 74.0f },    // size
        24, 24, 24, 24 };

    inline constexpr niketica::component::NineSlice NINE_SLICE_BUTTON_FOCUSED = {
        UI_TEXTURE,
        glm::vec2{ 602.0f, 74.0f },   // offset
        glm::vec2{ 212.0f, 74.0f },    // size
        24, 24, 24, 24 };

    inline constexpr niketica::component::NineSlice NINE_SLICE_BUTTON_PRESSED = {
        UI_TEXTURE,
        glm::vec2{ 602.0f, 148.0f },   // offset
        glm::vec2{ 212.0f, 74.0f },    // size
        24, 24, 24, 24 };

}
