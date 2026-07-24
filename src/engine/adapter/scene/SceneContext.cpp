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
        setCurrentScene(niketica::component::SceneType::USER_INTERFACE_DEMO);
    }

    void SceneContext::setCurrentScene(niketica::component::SceneType type)
    {
        auto it = sceneMap.find(type);
        if (it != sceneMap.end())
        {
            auto view = registry->view<entt::entity>(entt::exclude<niketica::component::Persistent>);
            for (auto entity : view)
            {
                registry->destroy(entity);
            }

            engineServices->getRenderContext()->reset();
            currentScene = it->second.get();
            currentScene->reset();
        }
        else
        {
            std::cerr << "ERROR::SceneContext::setCurrentScene - Unknown scene type" << std::endl;
        }
    }
}
