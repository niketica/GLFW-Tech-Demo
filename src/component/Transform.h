#pragma once

#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace niketica::component
{
    struct Transform
    {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 size = { 1.0f, 1.0f, 1.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f }; // Euler angles in degrees
    };

    struct LocalTransform
    {
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 size = { 1.0f, 1.0f, 1.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f }; // Euler angles in degrees
    };

    struct ParentTransform
    {
        entt::entity parent = entt::null;
    };

    struct SpriteOffsetPosition
    {
        glm::vec2 position = { 0.0f, 0.0f };
    };
}
