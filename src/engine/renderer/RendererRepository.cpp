#include "engine/renderer/RendererRepository.h"

namespace niketica::renderer
{
    
    RendererRepository::RendererRepository()
    {
        textureLoader = std::make_unique<TextureLoader>();
        simpleRenderer = std::make_unique<SimpleRenderer>();
        spriteInstancedRenderer = std::make_unique<SpriteInstancedRenderer>(textureLoader.get());
    }

}