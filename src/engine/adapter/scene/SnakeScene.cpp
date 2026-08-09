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
            {100.0f, 100.0f, 0.0f},
            {300.0f, 200.0f},
            {1, 0, 0, 1}
        };

        auto rect2 = niketica::component::Rectangle{
            {600.0f, 300.0f, 0.0f},
            {500.0f, 150.0f},
            {0, 1, 0, 1}
        };

        auto rect3 = niketica::component::Rectangle{
            {1200.0f, 200.0f, 0.0f},
            {200.0f, 500.0f},
            {0, 0, 1, 1}
        };
        registry->emplace<niketica::component::Rectangle>(registry->create(), rect1);
        registry->emplace<niketica::component::Rectangle>(registry->create(), rect2);
        registry->emplace<niketica::component::Rectangle>(registry->create(), rect3);

        auto rectBorder1 = niketica::component::RectangleBorder{
            {0.0f, 0.0f, 0.0f},
            {300.0f, 200.0f},
            {1, 1, 0, 1},
            {1, 1, 0, 1},
            5.0f,
            true
        };
        registry->emplace<niketica::component::RectangleBorder>(registry->create(), rectBorder1);

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

        auto viewRectBorder = registry->view<niketica::component::RectangleBorder>();
        for (auto entity : viewRectBorder)
        {
            const auto& rectBorder = viewRectBorder.get<niketica::component::RectangleBorder>(entity);
            engineServices->getRenderContext()->getRectangleBorderRenderer()->submit(rectBorder);
        }
    }
    
    void SnakeScene::render()
    {
        // For now just assume there is always exactly 1 active camera.
        auto viewCamera = registry->view<niketica::component::Camera, niketica::component::ActiveCamera>();
        const auto &camera = viewCamera.get<niketica::component::Camera>(viewCamera.front());

        systemContext->render();
        engineServices->getRenderContext()->getRectangleRenderer()->render(camera.projection, camera.view);
        engineServices->getRenderContext()->getRectangleBorderRenderer()->render(camera.projection, camera.view);
    }
    
    void SnakeScene::reset()
    {        
        systemContext.release();
        init();
    }

}
