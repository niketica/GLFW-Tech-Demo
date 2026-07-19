#pragma once

#include <memory>

#include "engine/core/renderer/IRenderContext.h"
#include "engine/core/asset/IAssetReader.h"
#include "engine/core/input/IInputContext.h"
#include "engine/core/sound/ISoundContext.h"

namespace niketica::engine
{

    class EngineServices
    {
    public:
        EngineServices
        (   
            std::unique_ptr<niketica::asset::IAssetReader> assetReader,
            std::unique_ptr<niketica::input::IInputContext> inputContext,
            std::unique_ptr<niketica::sound::ISoundContext> soundContext,
            std::unique_ptr<niketica::renderer::IRenderContext> renderContext
        ) :
            assetReader(std::move(assetReader)),
            inputContext(std::move(inputContext)),
            soundContext(std::move(soundContext)),
            renderContext(std::move(renderContext))
        {}

        ~EngineServices() = default;

        niketica::input::IInputContext* getInputContext() const
        {
            return inputContext.get();
        }

        niketica::sound::ISoundContext* getSoundContext() const
        {
            return soundContext.get();
        }

        niketica::renderer::IRenderContext* getRenderContext() const
        {
            return renderContext.get();
        }

    private:
        std::unique_ptr<niketica::asset::IAssetReader> assetReader;
        std::unique_ptr<niketica::input::IInputContext> inputContext;
        std::unique_ptr<niketica::sound::ISoundContext> soundContext;
        std::unique_ptr<niketica::renderer::IRenderContext> renderContext;

    };

}
