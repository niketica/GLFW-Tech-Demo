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

        auto viewViewport = registry->view<niketica::component::Viewport>();
        const auto& viewport = viewViewport.get<niketica::component::Viewport>(viewViewport.front());
        
        float size = 64.0f;

        gridEntities.clear();

        for (int x = 0; x < gridWidth; ++x)
        {
            for (int y = 0; y < gridHeight; ++y)
            {
                auto entity = createRectangleWithBorder
                (
                    { x * size, y * size, 0.0f },
                    { size, size },
                    { 0.1f, 0.1f, 0.1f, 1.0f },
                    { 1.0f, 1.0f, 1.0f, 1.0f },
                    1.0f,
                    1.0f
                );
                gridEntities.push_back(entity);
            }
        }
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
        if (input.actions[niketica::component::Action::W].pressed)
        {
            currentDirection = Direction::UP;
        }
        if (input.actions[niketica::component::Action::A].pressed)
        {
            currentDirection = Direction::LEFT;
        }
        if (input.actions[niketica::component::Action::S].pressed)
        {
            currentDirection = Direction::DOWN;
        }
        if (input.actions[niketica::component::Action::D].pressed)
        {
            currentDirection = Direction::RIGHT;
        }

    }

    void SnakeScene::update(float dt)
    {
        systemContext->update(dt);
        clearGrid();
        moveSnake(dt);
        colorSnake();
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
    
    entt::entity SnakeScene::createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill)
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
        
        return entity;
    }

    entt::entity SnakeScene::createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        auto transform = niketica::component::Transform{ position, {size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f} };
        auto fillColor = niketica::component::FillColor{ color };
        
        auto entity = registry->create();
        registry->emplace<niketica::component::Rectangle>(entity);
        registry->emplace<niketica::component::Transform>(entity, transform);
        registry->emplace<niketica::component::FillColor>(entity, fillColor);

        return entity;
    }

    entt::entity SnakeScene::getEntityAtGridPosition(int x, int y)
    {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight)
        {
            std::cerr << "ERROR::SnakeScene::getEntityAtGridPosition - Grid position out of bounds: (" << x << ", " << y << ")" << std::endl;
            return entt::null; // Return null entity if out of bounds
        }

        // int index = y * gridWidth + x;
        int index = x * gridHeight + y;
        return gridEntities[index];
    }

    void SnakeScene::clearGrid()
    {
        for (auto entity : gridEntities)
        {
            if (!registry->any_of<niketica::component::FillColor>(entity)) continue; // Skip if the entity doesn't have a FillColor component
            auto& fillColor = registry->get<niketica::component::FillColor>(entity);
            fillColor.color = { 0.1f, 0.1f, 0.1f, 1.0f };
        }
    }

    void SnakeScene::colorSnake()
    {
        // Color the snake head
        auto headEntity = getEntityAtGridPosition(snakeHeadPosition.x, snakeHeadPosition.y);
        if (headEntity != entt::null && registry->any_of<niketica::component::FillColor>(headEntity))
        {
            auto& fillColor = registry->get<niketica::component::FillColor>(headEntity);
            fillColor.color = { 0.0f, 1.0f, 0.0f, 1.0f }; // Green color for the head
        }

        // Color the snake body
        for (const auto& bodyPos : snakeBodyPositions)
        {
            auto bodyEntity = getEntityAtGridPosition(bodyPos.x, bodyPos.y);
            if (bodyEntity != entt::null && registry->any_of<niketica::component::FillColor>(bodyEntity))
            {
                auto& fillColor = registry->get<niketica::component::FillColor>(bodyEntity);
                fillColor.color = { 0.0f, 0.5f, 0.0f, 1.0f }; // Darker green for the body
            }
        }
    }

    void SnakeScene::moveSnake(float dt)
    {
        currentTimer += dt;
        if (currentTimer < cooldownPeriod) return;
        currentTimer = 0.0f;

        moveSnakeBody();
        moveSnakeHead();
    }

    void SnakeScene::moveSnakeHead()
    {
        // TODO these wrap around functions should really be game over
        switch (currentDirection)
        {
        case Direction::UP:
        {
            snakeHeadPosition.y++;
            if (snakeHeadPosition.y >= gridHeight)
            {
                snakeHeadPosition.y = 0;
            }
        }
        break;
        case Direction::DOWN:
        {
            snakeHeadPosition.y--;
            if (snakeHeadPosition.y < 0)
            {
                snakeHeadPosition.y = gridHeight - 1;
            }
        }
        break;
        case Direction::LEFT:
        {
            snakeHeadPosition.x--;
            if (snakeHeadPosition.x < 0)
            {
                snakeHeadPosition.x = gridWidth - 1;
            }
        }
        break;
        case Direction::RIGHT:
        {
            snakeHeadPosition.x++;
            if (snakeHeadPosition.x >= gridWidth)
            {
                snakeHeadPosition.x = 0;
            }
        }
        break;
        }
    }

    void SnakeScene::moveSnakeBody()
    {
        // TODO implement me
        // Just position the first body part on the snake head, then each body part on the body part before it
    }

}
