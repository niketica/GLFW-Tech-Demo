#include "scene/tetris/systems/VerticalMovementSystem.h"

namespace niketica::tetris
{

    void VerticalMovementSystem::update(float dt)
    {
        moveTetrominoDown(dt);
    }

    void VerticalMovementSystem::moveTetrominoDown(float dt)
    {
        bool autoMove = moveTetrominoDownAuto(dt);
        bool manualMove = moveTetrominoDownManual();

        if (autoMove || manualMove)
        {
            procesVerticalMovement();
        }
    }

    bool VerticalMovementSystem::moveTetrominoDownAuto(float dt)
    {
        auto& gameState = getGameState();
        gameState.currentTimer += dt;
        if (gameState.currentTimer < gameState.cooldownPeriod)
        {
            return false;
        }
        gameState.currentTimer = 0;

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        gridPosition.y--;
        return true;
    }

    bool VerticalMovementSystem::moveTetrominoDownManual()
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(viewTetromino.front());

        if (tetromino.direction != niketica::tetris::Direction::DOWN) return false;

        auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        gridPosition.y--;
        return true;
    }

    void VerticalMovementSystem::procesVerticalMovement()
    {
        auto& gameState = getGameState();
        gameState.verticalHitDetected = false;

        if (!isVerticalHit()) return;

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;

        gridPosition.y++;

        gameState.verticalHitDetected = true;
    }

    niketica::tetris::GameState& VerticalMovementSystem::getGameState()
    {
        auto viewGameState = registry->view<niketica::tetris::GameState>();
        return viewGameState.get<niketica::tetris::GameState>(viewGameState.front());
    }

    bool VerticalMovementSystem::isVerticalHit()
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions>();
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;

        auto& gameState = getGameState();

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            int y = gridPosition.y + blockPosition.y;
            if (y < 0)
            {
                return true;
            }
            else if (y < gameState.gridHeight)
            {
                auto grindBlockEntity = getEntityAtGridPosition(x, y);
                if (registry->any_of<niketica::tetris::SolidBlock>(grindBlockEntity)) return true;
            }
        }
        return false;
    }

    entt::entity VerticalMovementSystem::getEntityAtGridPosition(int x, int y)
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
