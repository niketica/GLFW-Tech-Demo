#pragma once

#include <string>
#include <glm/glm.hpp>

#include "component/Components.h"

namespace niketica::renderer
{
    struct RectangleBorderlessData
    {
        glm::vec3 position;
        glm::vec2 size;
        glm::vec4 color;
    };

    struct RectangleBorderData
    {
        glm::vec3 position;
        glm::vec2 size;
        glm::vec4 fillColor;
        glm::vec4 borderColor;
        float borderThickness;
        float fill; // 1.0f for filled, 0.0f for unfilled; floats are more straightforward for OpenGL.
    };

    class IRectangleRenderer
    {
    public:
        virtual ~IRectangleRenderer() = default;

        virtual void submit(const RectangleBorderlessData& rectangle) = 0;
        virtual void submit(const RectangleBorderData& rectangle) = 0;
        virtual void render(const glm::mat4& projection, const glm::mat4& view) = 0;
        virtual void clear() = 0;
        
    };
    
}
