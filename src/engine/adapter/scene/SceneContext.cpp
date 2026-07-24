#include "engine/adapter/scene/SceneContext.h"

namespace niketica::scene
{
    SceneContext::SceneContext(
            niketica::engine::EngineServices* engineServices
        ) : engineServices(engineServices)
    {}

    void SceneContext::initScenes()
    {
        sceneMap.emplace(niketica::component::SceneType::TEST, std::make_unique<TestScene>(registry, engineServices));
        sceneMap.emplace(niketica::component::SceneType::USER_INTERFACE_DEMO, std::make_unique<UserInterfaceDemoScene>(registry, engineServices));
        setCurrentScene(niketica::component::SceneType::TEST);
    }
}
