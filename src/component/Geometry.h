#pragma once

#include <glm/glm.hpp>

namespace niketica::component
{
    struct Rectangle
    {
        glm::vec3 position;
        glm::vec2 size;
        glm::vec4 color;
    };

    struct RectangleBorder
    {
        glm::vec3 position;
        glm::vec2 size;

        glm::vec4 fillColor;
        glm::vec4 borderColor;

        float borderThickness;

        bool fill;
    };
    
}
