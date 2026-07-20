#include "scene/SceneContext.h"

namespace niketica::scene
{
    SceneContext::SceneContext(
            niketica::engine::EngineServices* engineServices
        ) : engineServices(engineServices)
    {}

    void SceneContext::initScenes()
    {
        testScene = std::make_unique<TestScene>(registry, engineServices);
    }
}
