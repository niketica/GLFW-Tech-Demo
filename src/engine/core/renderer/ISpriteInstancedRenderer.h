#pragma once

#include <glm/glm.hpp>

#include "component/Components.h"

namespace niketica::renderer
{

    class ISpriteInstancedRenderer
    {
    public:
        virtual ~ISpriteInstancedRenderer() = default;

        virtual void clear() = 0;
        virtual void clearUI() = 0;

        virtual void submit(const unsigned int textureID, const niketica::component::Sprite& sprite, const glm::vec3& position, const glm::vec2& size,
            const float scale) = 0;
        virtual void submitNineSlice(
            const niketica::component::Transform& tr,
            const niketica::component::UINineSlice& ns,
            const niketica::component::NineSliceTexture& tex,
            const float scale) = 0;
            
        virtual void render(const glm::mat4& projection, const glm::mat4& view) = 0;
        virtual void renderUI(const glm::mat4& projection, const glm::mat4& view) = 0;
    };

}
