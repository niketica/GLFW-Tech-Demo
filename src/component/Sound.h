#pragma once

#include <string>

namespace niketica::component
{
    struct SoundEffect
    {
        std::string path = "";
    };

    struct Music
    {
        std::string path = "";
        bool loop = true;
    };

}
