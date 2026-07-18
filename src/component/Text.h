#pragma once

#include <string>
#include <glm/glm.hpp>

namespace component
{
    struct Text
    {
        std::string value;
        int fontSize = 16;
    };
}
