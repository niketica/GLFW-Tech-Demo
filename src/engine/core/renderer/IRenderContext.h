#pragma once

#include <memory>

#include "engine/core/renderer/ITextureLoader.h"
#include "engine/core/renderer/ISpriteInstancedRenderer.h"
#include "engine/core/renderer/INineSliceInstancedRenderer.h"
#include "engine/core/renderer/IRectangleRenderer.h"
#include "engine/core/renderer/IRectangleBorderRenderer.h"
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

        INineSliceInstancedRenderer* getNineSliceInstancedRenderer() const
        {
            return nineSliceInstancedRenderer.get();
        }

        ITextRenderer* getTextRenderer() const
        {
            return textRenderer.get();
        }

        IRectangleRenderer* getRectangleRenderer() const
        {
            return rectangleRenderer.get();
        }

        IRectangleBorderRenderer* getRectangleBorderRenderer() const
        {
            return rectangleBorderRenderer.get();
        }

        virtual void reset() = 0;

        virtual void setWindowMode(component::Window& windowComponent, component::WindowMode mode) = 0;
        virtual void setWindowSize(component::Window& windowComponent, int width, int height) = 0;
        virtual void updateViewport(component::Viewport& viewportComponent, const component::Window& windowComponent, const component::RenderSettings& renderSettings) = 0;
        virtual void updateCamera(component::Camera& camera, const component::RenderSettings& renderSettings) = 0;

    protected:
        std::unique_ptr<ITextureLoader> textureLoader;
        std::unique_ptr<ISpriteInstancedRenderer> spriteInstancedRenderer;
        std::unique_ptr<INineSliceInstancedRenderer> nineSliceInstancedRenderer;
        std::unique_ptr<ITextRenderer> textRenderer;
        std::unique_ptr<IRectangleRenderer> rectangleRenderer;
        std::unique_ptr<IRectangleBorderRenderer> rectangleBorderRenderer;

    };

}
