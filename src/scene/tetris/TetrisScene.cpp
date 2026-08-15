#include "scene/tetris/TetrisScene.h"

namespace niketica::scene
{
    TetrisScene::TetrisScene
    (
        entt::registry* registry,
        niketica::engine::EngineServices* engineServices
    ) : registry(registry), engineServices(engineServices)
    {
        init();
    }

    void TetrisScene::init()
    {
        systemContext = std::make_unique<niketica::systems::SystemContext>(registry, engineServices);
        systemContext->init();

        niketica::tetris::GameState gameState;
        gameState.gameActive = true;
        registry->emplace<niketica::tetris::GameState>(registry->create(), gameState);
        
        float windowWidth = 1920; // TODO Should be retrieved and updated dynamically
        float totalGridWidth = gameState.blockSize * gameState.gridWidth;
        float containerX = (windowWidth * 0.5f) - (totalGridWidth * 0.5f);
        auto containerEntity = registry->create();
        niketica::component::Transform containerTransform;
        containerTransform.position.x = containerX;
        registry->emplace<niketica::component::Transform>(containerEntity, containerTransform);
        
        for (int x = 0; x < gameState.gridWidth; ++x)
        {
            for (int y = 0; y < gameState.gridHeight; ++y)
            {
                auto entity = createRectangleWithBorder
                (
                    { x * gameState.blockSize, y * gameState.blockSize, 0.0f },
                    { gameState.blockSize, gameState.blockSize },
                    { 0.1f, 0.1f, 0.1f, 1.0f },
                    { 1.0f, 1.0f, 1.0f, 1.0f },
                    1.0f,
                    1.0f
                );
                niketica::component::LocalTransform local;
                local.position.x = x * gameState.blockSize;
                local.position.y = y * gameState.blockSize;
                local.size.x = gameState.blockSize;
                local.size.y = gameState.blockSize;
                registry->emplace<niketica::component::LocalTransform>(entity, niketica::component::LocalTransform{ local });
                registry->emplace<niketica::component::ParentTransform>(entity, niketica::component::ParentTransform{ containerEntity });
                registry->emplace<niketica::tetris::GridBlock>(entity);
                registry->emplace<niketica::tetris::GridPosition>(entity, niketica::tetris::GridPosition{ {x, y} });
            }
        }

        auto tetrominoCreationSystem = std::make_unique<niketica::tetris::TetrominoCreationSystem>(registry, engineServices);
        systemContext->addSystem(std::move(tetrominoCreationSystem));
        auto blockPositionSystem = std::make_unique<niketica::tetris::BlockPositionSystem>(registry, engineServices);
        systemContext->addSystem(std::move(blockPositionSystem));
        auto verticalMovementSystem = std::make_unique<niketica::tetris::VerticalMovementSystem>(registry, engineServices);
        systemContext->addSystem(std::move(verticalMovementSystem));

        registry->emplace<niketica::tetris::CreateRandomTetromino>(registry->create());
    }

    void TetrisScene::input()
    {
        systemContext->input();

        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::ESCAPE].pressed)
        {
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::MAIN_MENU };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }
        
        auto viewTetromino = registry->view<niketica::tetris::Tetromino>();
        auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(viewTetromino.front());
        tetromino.direction = niketica::tetris::Direction::UNDEFINED;
        if (input.actions[niketica::component::Action::A].pressed)
        {
            tetromino.direction = niketica::tetris::Direction::LEFT;
        }
        if (input.actions[niketica::component::Action::D].pressed)
        {
            tetromino.direction = niketica::tetris::Direction::RIGHT;
        }
        if (input.actions[niketica::component::Action::S].pressed)
        {
            tetromino.direction = niketica::tetris::Direction::DOWN;
        }
        if (input.actions[niketica::component::Action::E].pressed || input.actions[niketica::component::Action::R].pressed)
        {
            rotateTetromino();
        }

    }

    void TetrisScene::update(float dt)
    {
        systemContext->update(dt);

        auto& gameState = getGameState();

        if (gameState.gameActive)
        {
            clearGrid();
        }

        if (gameState.verticalHitDetected)
        {
            gameState.verticalHitDetected = false;
            moveTetrominoBlocksToGrid();
            registry->emplace<niketica::tetris::DestroyTetromino>(registry->create());
            registry->emplace<niketica::tetris::CreateRandomTetromino>(registry->create());
        }

        moveTetrominoHorizontal();
        colorTetrominoOnGrid();
    }
    
    void TetrisScene::render()
    {
        // For now just assume there is always exactly 1 active camera.
        auto viewCamera = registry->view<niketica::component::Camera, niketica::component::ActiveCamera>();
        const auto &camera = viewCamera.get<niketica::component::Camera>(viewCamera.front());

        systemContext->render();
        engineServices->getRenderContext()->getRectangleRenderer()->render(camera.projection, camera.view);
    }
    
    void TetrisScene::reset()
    {        
        systemContext.release();
        init();
    }
    
    entt::entity TetrisScene::createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill)
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

    entt::entity TetrisScene::createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        auto transform = niketica::component::Transform{ position, {size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f} };
        auto fillColor = niketica::component::FillColor{ color };
        
        auto entity = registry->create();
        registry->emplace<niketica::component::Rectangle>(entity);
        registry->emplace<niketica::component::Transform>(entity, transform);
        registry->emplace<niketica::component::FillColor>(entity, fillColor);

        return entity;
    }

    void TetrisScene::clearGrid()
    {
        auto viewGridBlocks = registry->view<niketica::tetris::GridBlock>();
        for (auto entity : viewGridBlocks)
        {
            if (registry->any_of<niketica::tetris::SolidBlock>(entity) || !registry->any_of<niketica::component::FillColor>(entity)) continue;
            auto& fillColor = registry->get<niketica::component::FillColor>(entity);
            fillColor.color = { 0.1f, 0.1f, 0.1f, 1.0f };
        }
    }

    void TetrisScene::rotateTetromino()
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition>();
        auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(viewTetromino.front());
        auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;

        switch (tetromino.rotation)
        {
        case niketica::tetris::Rotation::_1:
            tetromino.rotation = niketica::tetris::Rotation::_2;
            break;
        case niketica::tetris::Rotation::_2:
            tetromino.rotation = niketica::tetris::Rotation::_3;
            break;
        case niketica::tetris::Rotation::_3:
            tetromino.rotation = niketica::tetris::Rotation::_4;
            break;
        case niketica::tetris::Rotation::_4:
            tetromino.rotation = niketica::tetris::Rotation::_1;
            break;
        }

        registry->emplace<niketica::tetris::SetBlockPositions>(registry->create());

        while (shouldPushLeft())
        {
            gridPosition.x--;
        }

        while (shouldPushRight())
        {
            gridPosition.x++;
        }
    }

    void TetrisScene::colorTetrominoOnGrid()
    {
        const auto& gameState = getGameState();

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::BlockPositions, niketica::tetris::GridPosition, niketica::component::Color>();
        auto entity = viewTetromino.front();
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(entity).blockPositions;
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(entity).position;
        const auto& color = viewTetromino.get<niketica::component::Color>(entity).value;
        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            int y = gridPosition.y + blockPosition.y;
            if (x < 0 || x >= gameState.gridWidth || y < 0 || y >= gameState.gridHeight) continue;

            auto gridEntity = getEntityAtGridPositionTEMP(x, y);

            if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity) || !registry->any_of<niketica::component::FillColor>(gridEntity)) continue;
            auto& fillColor = registry->get<niketica::component::FillColor>(gridEntity);
            fillColor.color = color;
        }
    }
    
    void TetrisScene::moveTetrominoHorizontal()
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition>();
        const auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(viewTetromino.front());

        switch (tetromino.direction)
        {
        case niketica::tetris::Direction::LEFT:
        {
            if (canMoveLeft())
            {
                auto& position = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
                position.x--;
            }
        }
        break;
        case niketica::tetris::Direction::RIGHT:
        {
            if (canMoveRight())
            {
                auto& position = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
                position.x++;
            }
        }
        break;
        }
    }

    bool TetrisScene::canMoveLeft()
    {
        const auto& gameState = getGameState();

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x - 1;
            int y = gridPosition.y + blockPosition.y;
            if (x < 0)
            {
                return false;
            }
            else if (y < gameState.gridHeight)
            {
                auto gridEntity = getEntityAtGridPositionTEMP(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return false;
            }
        }

        return true;
    }

    bool TetrisScene::canMoveRight()
    {
        const auto& gameState = getGameState();

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x + 1;
            int y = gridPosition.y + blockPosition.y;
            if (x >= gameState.gridWidth)
            {
                return false;
            }
            else if (y < gameState.gridHeight)
            {
                auto gridEntity = getEntityAtGridPositionTEMP(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return false;
            }
        }

        return true;
    }

    bool TetrisScene::shouldPushLeft()
    {
        const auto& gameState = getGameState();

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            int y = gridPosition.y + blockPosition.y;
            if (x >= gameState.gridWidth)
            {
                return true;
            }
            else if (y < gameState.gridHeight)
            {
                auto gridEntity = getEntityAtGridPositionTEMP(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return true;
            }
        }

        return false;
    }

    bool TetrisScene::shouldPushRight()
    {
        const auto& gameState = getGameState();

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            int y = gridPosition.y + blockPosition.y;
            if (x < 0)
            {
                return true;
            }
            else if (y < gameState.gridHeight)
            {
                auto gridEntity = getEntityAtGridPositionTEMP(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return true;
            }
        }

        return false;
    }

    void TetrisScene::moveTetrominoBlocksToGrid()
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions, niketica::component::Color>();
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;
        const auto& color = viewTetromino.get<niketica::component::Color>(viewTetromino.front()).value;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            int y = gridPosition.y + blockPosition.y;

            auto grindBlockEntity = getEntityAtGridPositionTEMP(x, y);
            registry->emplace<niketica::tetris::SolidBlock>(grindBlockEntity);
            auto& fillColor = registry->get<niketica::component::FillColor>(grindBlockEntity);
            fillColor.color = color;
        }
    }

    niketica::tetris::GameState& TetrisScene::getGameState()
    {
        auto viewGameState = registry->view<niketica::tetris::GameState>();
        return viewGameState.get<niketica::tetris::GameState>(viewGameState.front());
    }

    entt::entity TetrisScene::getEntityAtGridPositionTEMP(int x, int y)
    {
        auto viewGridBlock = registry->view<niketica::tetris::GridBlock, niketica::tetris::GridPosition>();
        for (auto entity : viewGridBlock)
        {
            const auto& gridPosition = viewGridBlock.get<niketica::tetris::GridPosition>(entity).position;

            if (gridPosition.x == x && gridPosition.y == y)
            {
                return entity;
            }
        }

        std::cerr << "ERROR::TetrisScene::getEntityAtGridPositionTEMP - Grid block not found at: (" << x << ", " << y << ")" << std::endl;
        return entt::null; // Return null entity if out of bounds
    }

}
