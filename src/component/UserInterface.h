#pragma once

#include <string>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/TextureHandle.h"
#include "component/Input.h"

namespace niketica::component
{

    struct NineSlice
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

}
