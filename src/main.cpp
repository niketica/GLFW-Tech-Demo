#include <memory>

#include "engine/core/Engine.h"
#include "engine/core/renderer/IRenderContext.h"
#include "engine/adapter/renderer/RenderContext.h"
#include "engine/adapter/asset/PakReader.h"

int main()
{
    auto pakReader = std::make_unique<niketica::asset::PakReader>(niketica::asset::COMPRESSION_PASSPHRASE);
    niketica::asset::AssetManager::Get().RegisterLoader<niketica::asset::File>(std::make_shared<niketica::asset::FileLoader>(pakReader.get()));

    auto renderContext = std::make_unique<niketica::renderer::RenderContext>();
    niketica::engine::Engine engine(std::move(pakReader), std::move(renderContext));
    engine.start();
    return 0;
}
