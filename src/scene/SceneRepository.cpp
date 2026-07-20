#include "scene/SceneRepository.h"

namespace niketica::scene
{
    SceneRepository::SceneRepository(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        ) : registry(registry),
            engineServices(engineServices)
    {
        initScenes();
    }

    void SceneRepository::initScenes()
    {
        testScene = std::make_unique<TestScene>(registry, engineServices);
    }
}
