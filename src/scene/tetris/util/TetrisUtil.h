#pragma once

#include <iostream>
#include <entt/entt.hpp>

#include "scene/tetris/components/TetrisComponents.h"

namespace niketica::tetris
{

    static inline niketica::tetris::GameState& getGameState(entt::registry* registry)
    {
        auto viewGameState = registry->view<niketica::tetris::GameState>();
        return viewGameState.get<niketica::tetris::GameState>(viewGameState.front());
    }

    static inline entt::entity getEntityAtGridPosition(entt::registry* registry, int x, int y)
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
