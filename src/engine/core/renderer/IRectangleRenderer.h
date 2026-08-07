#pragma once

#include <string>
#include <glm/glm.hpp>

#include "component/Text.h"

namespace niketica::renderer
{

    class IRectangleRenderer
    {
    public:
        virtual ~IRectangleRenderer() = default;

        virtual void submit(const niketica::component::Rectangle& rectangle) = 0;
        virtual void render(const glm::mat4& projection, const glm::mat4& view) = 0;
        virtual void clear() = 0;
        
    };
    
}
