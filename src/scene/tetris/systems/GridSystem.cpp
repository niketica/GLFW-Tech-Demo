#include "scene/tetris/systems/GridSystem.h"

namespace niketica::tetris
{

    void GridSystem::update(float dt)
    {
        // Initial
        createGrid();

        // During gameplay
        moveTetrominoBlocksToGrid();
        clearGridLines();

        // Visual update
        clearGrid();
        colorTetrominoOnGrid();
    }

    void GridSystem::createGrid()
    {
        bool componentFound = false;
        auto view = registry->view<niketica::tetris::CreateGrid>();
        for (auto entity : view)
        {
            componentFound = true;
            registry->destroy(entity);
        }

        if (!componentFound) return;

        const auto& gameState = getGameState(registry);

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

    }
    
    entt::entity GridSystem::createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill)
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

    entt::entity GridSystem::createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
    {
        auto transform = niketica::component::Transform{ position, {size.x, size.y, 0.0f}, {1.0f, 1.0f, 1.0f} };
        auto fillColor = niketica::component::FillColor{ color };
        
        auto entity = registry->create();
        registry->emplace<niketica::component::Rectangle>(entity);
        registry->emplace<niketica::component::Transform>(entity, transform);
        registry->emplace<niketica::component::FillColor>(entity, fillColor);

        return entity;
    }

    void GridSystem::clearGrid()
    {
        auto viewGridBlocks = registry->view<niketica::tetris::GridBlock>();
        for (auto entity : viewGridBlocks)
        {
            if (registry->any_of<niketica::tetris::SolidBlock>(entity) || !registry->any_of<niketica::component::FillColor>(entity)) continue;
            auto& fillColor = registry->get<niketica::component::FillColor>(entity);
            fillColor.color = { 0.1f, 0.1f, 0.1f, 1.0f };
        }
    }

    void GridSystem::colorTetrominoOnGrid()
    {
        const auto& gameState = niketica::tetris::getGameState(registry);

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::BlockPositions, niketica::tetris::GridPosition, niketica::component::Color>();
        if (viewTetromino.begin() == viewTetromino.end()) return;

        auto entity = viewTetromino.front();
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(entity).blockPositions;
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(entity).position;
        const auto& color = viewTetromino.get<niketica::component::Color>(entity).value;
        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            int y = gridPosition.y + blockPosition.y;
            if (x < 0 || x >= gameState.gridWidth || y < 0 || y >= gameState.gridHeight) continue;

            auto gridEntity = niketica::tetris::getEntityAtGridPosition(registry, x, y);

            if (registry->any_of<niketica::tetris::SolidBlock>(gridEntity) || !registry->any_of<niketica::component::FillColor>(gridEntity)) continue;
            auto& fillColor = registry->get<niketica::component::FillColor>(gridEntity);
            fillColor.color = color;
        }
    }

    void GridSystem::moveTetrominoBlocksToGrid()
    {
        bool componentFound = false;
        auto view = registry->view<niketica::tetris::MoveTetrominoBlocksToGrid>();
        for (auto entity : view)
        {
            componentFound = true;
            registry->destroy(entity);
        }

        if (!componentFound) return;

        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::GridPosition, niketica::tetris::BlockPositions, niketica::component::Color>();
        const auto& gridPosition = viewTetromino.get<niketica::tetris::GridPosition>(viewTetromino.front()).position;
        const auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(viewTetromino.front()).blockPositions;
        const auto& color = viewTetromino.get<niketica::component::Color>(viewTetromino.front()).value;

        for (const auto& blockPosition : blockPositions)
        {
            int x = gridPosition.x + blockPosition.x;
            int y = gridPosition.y + blockPosition.y;

            auto grindBlockEntity = niketica::tetris::getEntityAtGridPosition(registry, x, y);
            registry->emplace<niketica::tetris::SolidBlock>(grindBlockEntity);
            auto& fillColor = registry->get<niketica::component::FillColor>(grindBlockEntity);
            fillColor.color = color;
        }
    }
    
    void GridSystem::clearGridLines()
    {
        const auto& gameState = getGameState(registry);
        for (int y=gameState.gridHeight - 1; y--; y>=0)
        {
            clearGridLine(y);
        }
    }

    void GridSystem::clearGridLine(int y)
    {
        const auto& gameState = getGameState(registry);
        int solidBlockCount = 0;
        for (int x=0; x<gameState.gridWidth; x++)
        {
            const auto entity = getEntityAtGridPosition(registry, x, y);
            if (registry->any_of<SolidBlock>(entity))
            {
                solidBlockCount++;
            }
        }

        if (solidBlockCount == gameState.gridWidth)
        {
            moveUpperGridLinesDown(y+1);
        }
    }
    
    void GridSystem::moveUpperGridLinesDown(int startY)
    {
        const auto& gameState = getGameState(registry);
        for (int y=startY; y<gameState.gridHeight; y++)
        {
            for (int x=0; x<gameState.gridWidth; x++)
            {
                const auto entityUpper = getEntityAtGridPosition(registry, x, y);
                const auto entityLower = getEntityAtGridPosition(registry, x, y-1);

                auto& colorLower = registry->get<niketica::component::FillColor>(entityLower);
                registry->remove<SolidBlock>(entityLower);

                if (registry->any_of<SolidBlock>(entityUpper))
                {
                    const auto& colorUpper = registry->get<niketica::component::FillColor>(entityUpper);
                    colorLower.color = colorUpper.color;
                    registry->emplace<SolidBlock>(entityLower);
                }
                else
                {
                    colorLower.color = { 0.1f, 0.1f, 0.1f, 1.0f };
                }
            }
        }
    }
    
}
