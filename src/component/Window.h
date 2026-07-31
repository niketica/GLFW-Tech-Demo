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

    struct RenderSettings
    {
        float virtualWidth = 1920.0f;
        float virtualHeight = 1080.0f;
    };
    
}
