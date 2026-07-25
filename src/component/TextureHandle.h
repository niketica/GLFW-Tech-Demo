#pragma once

#include <string>

namespace niketica::component
{
    struct TextureHandle
    {
        unsigned int id;
    };    

    struct NineSliceTexture
    {
        niketica::component::TextureHandle texture;
        float width;   // full texture width
        float height;  // full texture height
    };
}
