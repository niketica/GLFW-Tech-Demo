#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"
#include "scene/tetris/components/Tetrominoes.h"
#include "scene/tetris/components/TetrisComponents.h"

namespace niketica::tetris
{

    class TetrominoCreationSystem : public niketica::systems::ISystem
    {
    public:
        TetrominoCreationSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override;
        void render() override {};

    private:
        void createRandomTetromino();
        void destroyTetromino();
        entt::entity createTetrominoBase();
        void createTetrominoI();
        void createTetrominoJ();
        void createTetrominoL();
        void createTetrominoO();
        void createTetrominoS();
        void createTetrominoT();
        void createTetrominoZ();

        void procesCreate();
        void procesDestroy();

    };

}
