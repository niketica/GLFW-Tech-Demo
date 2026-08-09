#pragma once

#include <glm/glm.hpp>

namespace niketica::component
{
    struct Rectangle {};

    struct FillColor
    {
        glm::vec4 color;
    };

    struct BorderColor
    {
        glm::vec4 color;
    };

    struct BorderThickness
    {
        float thickness;
    };
    
}
