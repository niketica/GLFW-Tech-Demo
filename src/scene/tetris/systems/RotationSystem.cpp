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
        auto viewTetromino = registry->view<niketica::tetris::Tetromino>();
        auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(viewTetromino.front());

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
    }
    
}
