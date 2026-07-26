#pragma once

#include <glm/glm.hpp>

#include "component/Components.h"

namespace niketica::renderer
{

    class INineSliceInstancedRenderer
    {
    public:
        virtual ~INineSliceInstancedRenderer() = default;

        virtual void clear() = 0;
        virtual void submit(
            const niketica::component::Transform& tr,
            const niketica::component::NineSlice& ns,
            const niketica::component::NineSliceTexture& tex,
            const float scale) = 0;
        virtual void render(const glm::mat4& projection, const glm::mat4& view) = 0;
    };

}
