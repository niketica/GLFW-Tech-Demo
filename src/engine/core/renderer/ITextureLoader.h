#pragma once

#include "component/TextureHandle.h"

namespace niketica::renderer
{
    class ITextureLoader
    {
    public:
        virtual ~ITextureLoader() = default;

        virtual niketica::component::TextureHandle acquire(const char* path, bool compressed = true) = 0;
        virtual unsigned int getAdapterTextureID(unsigned int textureID) = 0;

    };

}
