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

        createRectangleBorderless({ 100.0f, 100.0f, 0.0f  }, { 300.0f, 200.0f }, { 1, 0, 0, 1 });
        createRectangleBorderless({ 600.0f, 300.0f, 0.0f  }, { 500.0f, 150.0f }, { 0, 1, 0, 1 });
        createRectangleBorderless({ 1200.0f, 200.0f, 0.0f }, { 200.0f, 500.0f }, { 0, 0, 1, 1 });

        createRectangleWithBorder
        (
            { 400.0f, 400.0f, 0.0f },
            { 200.0f, 200.0f },
            { 1, 0, 1, 1 },
            { 1, 1, 1, 1 },
            10.0f,
            1.0f
        );
        createRectangleWithBorder
        (
            { 700.0f, 100.0f, 0.0f },
            { 300.0f, 300.0f },
            { 0, 1, 0, 1 },
            { 1, 1, 1, 1 },
            10.0f,
            0.0f
        );
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

        auto viewRect = registry->view<niketica::component::Rectangle, niketica::component::Transform>();
        for (auto entity : viewRect)
        {
            const bool hasBorder = registry->all_of<niketica::component::BorderColor, niketica::component::BorderThickness>(entity);
            const bool hasFill = registry->all_of<niketica::component::FillColor>(entity);

            const auto& transform = viewRect.get<niketica::component::Transform>(entity);

            if (hasBorder)
            {
                const auto& borderColor = registry->get<niketica::component::BorderColor>(entity);
                const auto& borderThickness = registry->get<niketica::component::BorderThickness>(entity);
                if (hasFill)
                {
                    const auto& fillColor = registry->get<niketica::component::FillColor>(entity);
                    auto rectBorderData = niketica::renderer::RectangleBorderData{
                        transform.position,
                        transform.size,
                        fillColor.color,
                        borderColor.color,
                        borderThickness.thickness,
                        1.0f
                    };
                    renderer->submit(rectBorderData);                    
                }
                else
                {
                    auto rectBorderData = niketica::renderer::RectangleBorderData{
                        transform.position,
                        transform.size,
                        {},
                        borderColor.color,
                        borderThickness.thickness,
                        0.0f
                    };
                    renderer->submit(rectBorderData);
                }
            }
            else if (hasFill)
            {
                const auto& fillColor = registry->get<niketica::component::FillColor>(entity);
                auto rectData = niketica::renderer::RectangleBorderlessData{
                    transform.position,
                    transform.size,
                    fillColor.color
                };
                renderer->submit(rectData);                
            }
        }
    }
    
    void SnakeScene::render()
    {
        // For now just assume there is always exactly 1 active camera.
        auto viewCamera = registry->view<niketica::component::Camera, niketica::component::ActiveCamera>();
        const auto &camera = viewCamera.get<niketica::component::Camera>(viewCamera.front());

        systemContext->render();
        engineServices->getRenderContext()->getRectangleRenderer()->render(camera.projection, camera.view);
    }
    
    void SnakeScene::reset()
    {        
        systemContext.release();
        init();
    }
    
    void SnakeScene::createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill)
    {
        auto transform = niketica::component::Transform{ position, {size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f} };
        auto fillColorCmpnt = niketica::component::FillColor{ fillColor };
        auto borderColorCmpnt = niketica::component::BorderColor{ borderColor };
        auto borderThicknessCmpnt = niketica::component::BorderThickness{ borderThickness };
        
        auto entity = registry->create();
        registry->emplace<niketica::component::Rectangle>(entity);
        registry->emplace<niketica::component::Transform>(entity, transform);
        registry->emplace<niketica::component::BorderColor>(entity, borderColorCmpnt);
        registry->emplace<niketica::component::BorderThickness>(entity, borderThicknessCmpnt);

        if (fill > 0.0f)
        {
            registry->emplace<niketica::component::FillColor>(entity, fillColorCmpnt);
        }
    }

    void SnakeScene::createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        auto transform = niketica::component::Transform{ position, {size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f} };
        auto fillColor = niketica::component::FillColor{ color };
        
        auto entity = registry->create();
        registry->emplace<niketica::component::Rectangle>(entity);
        registry->emplace<niketica::component::Transform>(entity, transform);
        registry->emplace<niketica::component::FillColor>(entity, fillColor);
    }

}
