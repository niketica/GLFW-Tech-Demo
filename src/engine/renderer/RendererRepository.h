#pragma once

#include <memory>

#include "engine/renderer/TextureLoader.h"
#include "engine/renderer/SimpleRenderer.h"
#include "engine/renderer/SpriteInstancedRenderer.h"
#include "engine/renderer/text/TextRenderer.h"

namespace niketica::renderer
{
    class RendererRepository
    {
    public:
        RendererRepository();
        ~RendererRepository() = default;

        TextureLoader* getTextureLoader() const { return textureLoader.get(); }
        SimpleRenderer* getSimpleRenderer() const { return simpleRenderer.get(); }
        SpriteInstancedRenderer* getSpriteInstancedRenderer() const { return spriteInstancedRenderer.get(); }
        TextRenderer* getTextRenderer() const { return textRenderer.get(); }

    private:
        std::unique_ptr<TextureLoader> textureLoader;
        std::unique_ptr<SimpleRenderer> simpleRenderer;
        std::unique_ptr<SpriteInstancedRenderer> spriteInstancedRenderer;
        std::unique_ptr<TextRenderer> textRenderer;
        
    };
}