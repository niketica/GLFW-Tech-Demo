#pragma once

#include <glm/glm.hpp>

namespace niketica::component
{
    enum class WindowMode
    {
        UNDEFINED,
        WINDOWED,
        BORDERLESS,
        FULLSCREEN
    };

    struct Window
    {
        WindowMode mode = WindowMode::UNDEFINED;
        int x;
        int y;
        int width;
        int height;
        glm::mat4 projection;
        float scale = 1.0f;
        glm::mat4 view;
    };

    struct NextWindowState
    {
        WindowMode mode;
        int width;
        int height;
    };
}
