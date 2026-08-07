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
        systemContext->input();

        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::ESCAPE].pressed)
        {
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::MAIN_MENU };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }

    }

    void SnakeScene::update(float deltaTime)
    {
        systemContext->update(deltaTime);

        auto renderer = engineServices->getRenderContext()->getRectangleRenderer();
        renderer->clear();
        renderer->submit({
            { -0.8f, -0.5f },
            { 0.3f, 0.4f },
            { 1,0,0,1 }});
        renderer->submit({
            { 0.1f, 0.3f },
            { 0.6f, 0.2f },
            { 0,1,0,1 }});
        renderer->submit({
            { -0.2f, -0.2f },
            { 0.2f, 0.7f },
            { 0,0,1,1 }});

    }
    
    void SnakeScene::render()
    {
        systemContext->render();
        engineServices->getRenderContext()->getRectangleRenderer()->render();
    }
    
    void SnakeScene::reset()
    {        
        systemContext.release();
        init();
    }

}
