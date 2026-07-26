#pragma once

#include <string>
#include <glm/glm.hpp>

namespace niketica::component
{

    enum class FontType
    {
        OPEN_SANS_REGULAR,
        COURIER_PRIME_CODE
    };
    
    struct Text
    {
        std::string value = "";
        int fontSize = 16;
        glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        float scale = 1.0f;
        FontType fontType = FontType::OPEN_SANS_REGULAR;
    };
}
