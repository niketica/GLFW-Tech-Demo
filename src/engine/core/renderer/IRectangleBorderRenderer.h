#pragma once

#include <string>
#include <glm/glm.hpp>

#include "component/Components.h"

namespace niketica::renderer
{

    class IRectangleBorderRenderer
    {
    public:
        virtual ~IRectangleBorderRenderer() = default;

        virtual void submit(const niketica::component::RectangleBorder& rectangle) = 0;
        virtual void render(const glm::mat4& projection, const glm::mat4& view) = 0;
        virtual void clear() = 0;
        
    };
    
}
