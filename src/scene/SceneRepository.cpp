#include "scene/SceneRepository.h"

namespace niketica::scene
{
    SceneRepository::SceneRepository(
            entt::registry* registry,
            niketica::systems::SystemRepository* systemRepository,
            niketica::renderer::IRenderContext* renderContext
        ) : registry(registry),
            systemRepository(systemRepository),
            renderContext(renderContext)
    {
        initScenes();
    }

    void SceneRepository::initScenes()
    {
        testScene = std::make_unique<TestScene>(registry, systemRepository, renderContext);
    }
}
