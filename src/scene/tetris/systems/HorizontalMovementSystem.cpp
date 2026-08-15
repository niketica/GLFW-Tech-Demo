#include "scene/tetris/systems/HorizontalMovementSystem.h"

namespace niketica::tetris
{

    void HorizontalMovementSystem::update(float dt)
    {
        moveTetrominoHorizontal();
    }
    
    void HorizontalMovementSystem::moveTetrominoHorizontal()
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

    bool HorizontalMovementSystem::canMoveLeft()
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
                auto gridEntity = getEntityAtGridPosition(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return false;
            }
        }

        return true;
    }

    bool HorizontalMovementSystem::canMoveRight()
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
                auto gridEntity = getEntityAtGridPosition(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity)) return false;
            }
        }

        return true;
    }

    niketica::tetris::GameState& HorizontalMovementSystem::getGameState()
    {
        auto viewGameState = registry->view<niketica::tetris::GameState>();
        return viewGameState.get<niketica::tetris::GameState>(viewGameState.front());
    }

    entt::entity HorizontalMovementSystem::getEntityAtGridPosition(int x, int y)
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
