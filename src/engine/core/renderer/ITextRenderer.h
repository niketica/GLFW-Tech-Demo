#pragma once

#include <string>
#include <glm/glm.hpp>

namespace niketica::renderer
{

    enum class FontType
    {
        OPEN_SANS_REGULAR
    };

    class ITextRenderer
    {
    public:
        virtual ~ITextRenderer() = default;

        virtual void begin(const glm::mat4& projection, const FontType& font) = 0;
        virtual void submitText(const FontType& font, const std::string& text,
            glm::vec2 pos, float scale, glm::vec4 color) = 0;
        virtual void flush() = 0;
    };
    
}
