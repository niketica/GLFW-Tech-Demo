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

        auto rect1 = niketica::component::Rectangle{
            { -0.8f, -0.5f, 0.0f },
            { 0.3f, 0.4f },
            { 1,0,0,1 }
        };
        auto rect2 = niketica::component::Rectangle{
            { 0.1f, 0.3f, 0.0f },
            { 0.6f, 0.2f },
            { 0,1,0,1 }
        };
        auto rect3 = niketica::component::Rectangle{
            { -0.2f, -0.2f, 0.0f },
            { 0.2f, 0.7f },
            { 0,0,1,1 }
        };
        registry->emplace<niketica::component::Rectangle>(registry->create(), rect1);
        registry->emplace<niketica::component::Rectangle>(registry->create(), rect2);
        registry->emplace<niketica::component::Rectangle>(registry->create(), rect3);
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

        auto viewRect = registry->view<niketica::component::Rectangle>();
        for (auto entity : viewRect)
        {
            const auto& rect = viewRect.get<niketica::component::Rectangle>(entity);
            renderer->submit(rect);
        }
    }
    
    void SnakeScene::render()
    {
        // For now just assume there is always exactly 1 active camera.
        auto viewCamera = registry->view<niketica::component::Camera, niketica::component::ActiveCamera>();
        const auto &camera = viewCamera.get<niketica::component::Camera>(viewCamera.front());

        glm::mat4 projection = glm::ortho(
            -1.0f,  // left
            1.0f,  // right
            -1.0f,  // bottom
            1.0f   // top
        );

        glm::mat4 view = glm::mat4(1.0f);

        systemContext->render();
        engineServices->getRenderContext()->getRectangleRenderer()->render(projection, view);
    }
    
    void SnakeScene::reset()
    {        
        systemContext.release();
        init();
    }

}
