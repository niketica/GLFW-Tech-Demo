#include "scene/tetris/systems/RotationSystem.h"

namespace niketica::tetris
{

    void RotationSystem::update(float dt)
    {
        bool componentFound = false;
        auto view = registry->view<niketica::tetris::RotateTetromino>();
        for (auto entity : view)
        {
            componentFound = true;
            registry->destroy(entity);
        }

        if (componentFound)
        {
            rotateTetromino();
        }
        
    }

    void RotationSystem::rotateTetromino()
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

    bool RotationSystem::shouldPushLeft()
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
                auto gridEntity = getEntityAtGridPosition(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return true;
            }
        }

        return false;
    }

    bool RotationSystem::shouldPushRight()
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
                auto gridEntity = getEntityAtGridPosition(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return true;
            }
        }

        return false;
    }

    niketica::tetris::GameState& RotationSystem::getGameState()
    {
        auto viewGameState = registry->view<niketica::tetris::GameState>();
        return viewGameState.get<niketica::tetris::GameState>(viewGameState.front());
    }

    entt::entity RotationSystem::getEntityAtGridPosition(int x, int y)
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

        std::cerr << "ERROR::TetrisScene::getEntityAtGridPosition - Grid block not found at: (" << x << ", " << y << ")" << std::endl;
        return entt::null; // Return null entity if out of bounds
    }
    
}
