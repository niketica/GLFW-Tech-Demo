#include "scene/SceneRepository.h"

namespace niketica::scene
{
    SceneRepository::SceneRepository(
            entt::registry* registry,
            niketica::systems::SystemRepository* systemRepository,
            niketica::renderer::RendererRepository* rendererRepository
        ) : registry(registry),
            systemRepository(systemRepository),
            rendererRepository(rendererRepository)
    {
        initScenes();
    }

    void SceneRepository::initScenes()
    {
        testScene = std::make_unique<TestScene>(registry, systemRepository, rendererRepository);
    }
}
