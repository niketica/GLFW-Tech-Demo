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
        int width;
        int height;
        WindowMode mode = WindowMode::UNDEFINED;
    };

    struct Viewport
    {
        int x;
        int y;
        int width;
        int height;
        float scale;
    };

    struct NextWindowState
    {
        WindowMode mode;
        int width;
        int height;
    };
}
