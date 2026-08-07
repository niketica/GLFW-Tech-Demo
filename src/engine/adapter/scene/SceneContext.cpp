#include "engine/adapter/scene/SceneContext.h"

namespace niketica::scene
{
    SceneContext::SceneContext(
            niketica::engine::EngineServices* engineServices
        ) : engineServices(engineServices)
    {}

    void SceneContext::initScenes()
    {
        sceneMap.emplace(niketica::component::SceneType::MAIN_MENU, std::make_unique<MainMenuScene>(registry, engineServices));
        sceneMap.emplace(niketica::component::SceneType::SNAKE, std::make_unique<SnakeScene>(registry, engineServices));
        setCurrentScene(niketica::component::SceneType::MAIN_MENU);
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

    void SceneContext::input() { currentScene->input(); };

    void SceneContext::update(float dt) {

        auto viewSceneSwitch = registry->view<niketica::component::SceneSwitchInstruction>();
        if (!viewSceneSwitch.empty())
        {
            auto entity = viewSceneSwitch.front();
            auto& sceneSwitch = registry->get<niketica::component::SceneSwitchInstruction>(entity);
            setCurrentScene(sceneSwitch.nextScene);
        }

        currentScene->update(dt);
    };

    void SceneContext::render() { currentScene->render(); };

}
