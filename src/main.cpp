#include <memory>

#include "engine/core/Engine.h"
#include "engine/core/EngineServices.h"
#include "engine/core/renderer/IRenderContext.h"
#include "engine/adapter/asset/AssetManager.h"
#include "engine/adapter/asset/PakReader.h"
#include "engine/adapter/asset/FileLoader.h"
#include "engine/adapter/renderer/RenderContext.h"
#include "engine/adapter/asset/PakReader.h"
#include "engine/adapter/input/InputBackendGLFW.h"
#include "engine/adapter/sound/SoundBackendMiniaudio.h"

int main()
{
    auto pakReader = std::make_unique<niketica::asset::PakReader>(niketica::asset::COMPRESSION_PASSPHRASE);
    niketica::asset::AssetManager::Get().RegisterLoader<niketica::asset::File>(std::make_shared<niketica::asset::FileLoader>(pakReader.get()));

    auto renderContext = std::make_unique<niketica::renderer::RenderContext>();
    renderContext->init();

    auto inputContext = std::make_unique<niketica::input::InputBackendGLFW>(renderContext->getWindow());

    auto soundBackend = std::make_unique<niketica::sound::SoundBackendMiniaudio>();
    soundBackend->init();

    auto engineServices = std::make_unique<niketica::engine::EngineServices>(std::move(pakReader), std::move(inputContext), std::move(soundBackend), std::move(renderContext));

    niketica::engine::Engine engine(std::move(engineServices));
    engine.start();
    return 0;
}
