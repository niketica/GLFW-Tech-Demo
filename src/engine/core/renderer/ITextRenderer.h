#pragma once

#include <string>
#include <glm/glm.hpp>

#include "component/Text.h"

namespace niketica::renderer
{

    class ITextRenderer
    {
    public:
        virtual ~ITextRenderer() = default;

        virtual void begin(const glm::mat4& projection, const niketica::component::FontType& font) = 0;
        virtual void submitText(const niketica::component::FontType& font, const std::string& text,
            glm::vec2 pos, float scale, glm::vec4 color) = 0;
        virtual void flush() = 0;
    };
    
}
