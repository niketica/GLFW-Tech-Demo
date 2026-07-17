# pragma once

#include <cstdint>
#include <glad/glad.h>
#include <vector>

#include "component/TextureArray.h"

namespace niketica::renderer
{

    struct TextureArray
    {
        GLuint glID = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t maxLayers = 0;
        uint32_t mipLevels = 1;

        component::TextureArrayFormat format;

        std::vector<bool> layerUsed;
    };
}
