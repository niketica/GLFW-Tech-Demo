#include "engine/renderer/RendererRepository.h"

namespace niketica::renderer
{
    
    RendererRepository::RendererRepository()
    {
        textureLoader = std::make_unique<TextureLoader>();
        simpleRenderer = std::make_unique<SimpleRenderer>();
        spriteInstancedRenderer = std::make_unique<SpriteInstancedRenderer>(textureLoader.get());
        textRenderer = std::make_unique<TextRenderer>(1920.0f, 1080.0f); // Just give the dimensions hard coded for now
        textRenderer->init();
    }

}