#include "scene/snake/SnakeScene.h"

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

        gridEntities.clear();
        snakeHitWall = false;
        snakeBodyPositions.clear();
        snakeHeadPosition = { 5, 5 };
        fruitActive = false;
        currentDirection = Direction::RIGHT;
        gameActive = true;

        float size = 64.0f;
        
        float windowWidth = 1920; // TODO Should be retrieved and updated dynamically
        float totalGridWidth = size * gridWidth;
        float containerX = (windowWidth * 0.5f) - (totalGridWidth * 0.5f);
        auto containerEntity = registry->create();
        niketica::component::Transform containerTransform;
        containerTransform.position.x = containerX;
        registry->emplace<niketica::component::Transform>(containerEntity, containerTransform);
        
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
                niketica::component::LocalTransform local;
                local.position.x = x * size;
                local.position.y = y * size;
                local.size.x = size;
                local.size.y = size;
                registry->emplace<niketica::component::LocalTransform>(entity, niketica::component::LocalTransform{ local });
                registry->emplace<niketica::component::ParentTransform>(entity, niketica::component::ParentTransform{ containerEntity });
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

        if (gameActive)
        {
            moveSnake(dt);
            checkSnakeEatsFruit();
            createFruit();
            clearGrid();
            colorFruit();
            colorSnake();
        }
    }
    
    void SnakeScene::render()
    {
        systemContext->render();
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

        // Color the snake head
        auto headEntity = getEntityAtGridPosition(snakeHeadPosition.x, snakeHeadPosition.y);
        if (headEntity != entt::null && registry->any_of<niketica::component::FillColor>(headEntity))
        {
            auto& fillColor = registry->get<niketica::component::FillColor>(headEntity);
            fillColor.color = { 0.0f, 1.0f, 0.0f, 1.0f }; // Green color for the head
        }
    }

    void SnakeScene::moveSnake(float dt)
    {
        currentTimer += dt;
        if (currentTimer < cooldownPeriod) return;
        currentTimer = 0.0f;

        moveSnakeBody();
        moveSnakeHead();
        checkLoseConditions();
    }

    void SnakeScene::moveSnakeHead()
    {
        switch (currentDirection)
        {
        case Direction::UP:
        {
            snakeHeadPosition.y++;
            if (snakeHeadPosition.y >= gridHeight)
            {
                snakeHeadPosition.y--;
                snakeHitWall = true;
            }
        }
        break;
        case Direction::DOWN:
        {
            snakeHeadPosition.y--;
            if (snakeHeadPosition.y < 0)
            {
                snakeHeadPosition.y = 0;
                snakeHitWall = true;
            }
        }
        break;
        case Direction::LEFT:
        {
            snakeHeadPosition.x--;
            if (snakeHeadPosition.x < 0)
            {
                snakeHeadPosition.x = 0;
                snakeHitWall = true;
            }
        }
        break;
        case Direction::RIGHT:
        {
            snakeHeadPosition.x++;
            if (snakeHeadPosition.x >= gridWidth)
            {
                snakeHeadPosition.x--;
                snakeHitWall = true;
            }
        }
        break;
        }
    }

    void SnakeScene::moveSnakeBody()
    {
        bool first = true;
        glm::ivec2 previous;
        for (auto& bodyPosition : snakeBodyPositions)
        {
            glm::ivec2 current = { bodyPosition.x, bodyPosition.y };
            if (first)
            {
                first = false;
                bodyPosition = { snakeHeadPosition.x, snakeHeadPosition.y };
            }
            else
            {
                bodyPosition = { previous.x, previous.y };
            }
            previous = { current.x, current.y };
        }
    }

    void SnakeScene::createFruit()
    {
        if (fruitActive) return;
        fruitActive = true;

        bool validPosition = false;

        // Defensive measure
        int maxAttempts = gridWidth * gridHeight;
        int currentAttempt = 0;

        while (!validPosition)
        {
            currentAttempt++;
            if (currentAttempt > maxAttempts)
            {
                std::cerr << "ERROR::SnakeScene::createFruit - Max number of attemps reached." << std::endl;
                return;
            }

            int x = rand() % gridWidth;
            int y = rand() % gridHeight;

            if (x == snakeHeadPosition.x && y == snakeHeadPosition.y) continue;

            for (const auto& bodyPosition : snakeBodyPositions)
            {
                if (x == bodyPosition.x && y == bodyPosition.y) continue;
            }
            fruitPosition = { x, y };
            validPosition = true;
        }

    }

    void SnakeScene::colorFruit()
    {
        auto entity = getEntityAtGridPosition(fruitPosition.x, fruitPosition.y);
        if (entity != entt::null && registry->any_of<niketica::component::FillColor>(entity))
        {
            auto& fillColor = registry->get<niketica::component::FillColor>(entity);
            fillColor.color = { 0.6f, 0.1f, 0.1f, 1.0f };
        }
    }

    void SnakeScene::checkSnakeEatsFruit()
    {
        bool hit = snakeHeadPosition.x == fruitPosition.x && snakeHeadPosition.y == fruitPosition.y;
        if (hit)
        {
            fruitActive = false;
            snakeBodyPositions.push_back({ snakeHeadPosition.x, snakeHeadPosition.y });
        }
    }

    void SnakeScene::checkLoseConditions()
    {
        if (snakeHitWall || snakeHitBody())
        {
            gameActive = false;
            std::cout << "INFO::SnakeScene::checkVictoryConditions - You lose!" << std::endl;
        }
    }

    bool SnakeScene::snakeHitBody() const
    {
        for (const auto& bodyPosition : snakeBodyPositions)
        {
            if (bodyPosition.x == snakeHeadPosition.x && bodyPosition.y == snakeHeadPosition.y)
            {
                return true;
            }
        }
        return false;
    }

}
