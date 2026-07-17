#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

#include "component/TextureHandle.h"

namespace niketica::component
{
    struct Sprite
    {
        glm::vec4 uv = { 0.0f, 0.0f, 1.0f, 1.0f };
        bool dynamicScale = false;
    };

    struct SpawnSprite
    {
        float x;
        float y;
        float width;
        float height;
        const char* path;
        float z;
        bool dynamicScale;
        float scale = 1.0f;
    };

    struct RenderSprite {};

    struct Static {};

    struct Baked {};
}
