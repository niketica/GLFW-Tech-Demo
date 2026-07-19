#include "engine/adapter/renderer/RenderContext.h"

namespace niketica::renderer
{

    void RenderContext::init()
    {
        textureLoader = std::make_unique<TextureLoader>();
        spriteInstancedRenderer = std::make_unique<SpriteInstancedRenderer>(textureLoader.get());
        textRenderer = std::make_unique<TextRenderer>(1920.0f, 1080.0f); // Just give the dimensions hard coded for now
    }

}
