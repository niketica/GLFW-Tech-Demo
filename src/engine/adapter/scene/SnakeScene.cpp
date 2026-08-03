#include "engine/adapter/scene/SnakeScene.h"

namespace niketica::scene
{
    SnakeScene::SnakeScene
    (
        entt::registry* registry,
        niketica::engine::EngineServices* engineServices
    ) : registry(registry), engineServices(engineServices)
    {
        init();
    }

    void SnakeScene::init()
    {
        systemContext = std::make_unique<niketica::systems::SystemContext>(registry, engineServices);
        systemContext->init();
    }

    void SnakeScene::input()
    {

    }

    void SnakeScene::update(float deltaTime)
    {

    }
    
    void SnakeScene::render()
    {

    }
    
    void SnakeScene::reset()
    {

    }

}
