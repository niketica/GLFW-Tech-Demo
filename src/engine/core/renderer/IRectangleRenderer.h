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

        virtual void render() = 0;
        
    };
    
}
