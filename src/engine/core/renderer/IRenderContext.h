#pragma once

#include <memory>

#include "engine/core/renderer/ITextureLoader.h"
#include "engine/core/renderer/ISpriteInstancedRenderer.h"
#include "engine/core/renderer/ITextRenderer.h"

namespace niketica::renderer
{

    class IRenderContext
    {
    public:
        virtual ~IRenderContext() = default;

        virtual void init() = 0;

        virtual bool windowShouldClose() = 0;
        virtual void startFrame() = 0;
        virtual void endFrame() = 0;
        virtual void setWindowTitle(const char* title) = 0;

        ITextureLoader* getTextureLoader() const
        {
            return textureLoader.get();
        }

        ISpriteInstancedRenderer* getSpriteInstancedRenderer() const
        {
            return spriteInstancedRenderer.get();
        }

        ITextRenderer* getTextRenderer() const
        {
            return textRenderer.get();
        }

        virtual void reset() = 0;
        
    protected:
        std::unique_ptr<ITextureLoader> textureLoader;
        std::unique_ptr<ISpriteInstancedRenderer> spriteInstancedRenderer;
        std::unique_ptr<ITextRenderer> textRenderer;

    };

}
