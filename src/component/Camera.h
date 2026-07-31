#pragma once

#include <glm/glm.hpp>

namespace niketica::component
{

    struct Camera
    {
        glm::mat4 projection;
        glm::mat4 view;
        float virtualWidth;
        float virtualHeight;
        float zoom;
    };

    struct ActiveCamera {};

}
