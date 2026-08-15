#include "scene/tetris/systems/TetrominoCreationSystem.h"

namespace niketica::tetris
{

    void TetrominoCreationSystem::update(float dt)
    {
        procesDestroy();
        procesCreate();
    }

    void TetrominoCreationSystem::procesCreate()
    {
        bool componentFound = false;
        auto viewSetBlockPositions = registry->view<niketica::tetris::CreateRandomTetromino>();
        for (auto entity : viewSetBlockPositions)
        {
            componentFound = true;
            registry->destroy(entity);
        }

        if (componentFound)
        {
            createRandomTetromino();
        }
    }

    void TetrominoCreationSystem::procesDestroy()
    {
        bool componentFound = false;
        auto viewSetBlockPositions = registry->view<niketica::tetris::DestroyTetromino>();
        for (auto entity : viewSetBlockPositions)
        {
            componentFound = true;
            registry->destroy(entity);
        }

        if (componentFound)
        {
            destroyTetromino();
        }
    }
    
    void TetrominoCreationSystem::createRandomTetromino()
    {
        int index = rand() % 7;
        switch (index)
        {
        case 0:
            createTetrominoI();
            break;
        case 1:
            createTetrominoJ();
            break;
        case 2:
            createTetrominoL();
            break;
        case 3:
            createTetrominoO();
            break;
        case 4:
            createTetrominoS();
            break;
        case 5:
            createTetrominoT();
            break;
        case 6:
            createTetrominoZ();
            break;
        }
    }
    
    void TetrominoCreationSystem::createTetrominoI()
    {
        auto entity = createTetrominoBase();

        auto matrices = niketica::tetris::Matrices4X4
        {
            niketica::tetris::TETROMINO_I_1,            
            niketica::tetris::TETROMINO_I_2,            
            niketica::tetris::TETROMINO_I_3,
            niketica::tetris::TETROMINO_I_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_I);
        registry->emplace<niketica::tetris::Matrices4X4>(entity, matrices);
    }

    void TetrominoCreationSystem::createTetrominoJ()
    {
        auto entity = createTetrominoBase();

        auto matrices = niketica::tetris::Matrices3X3
        {
            niketica::tetris::TETROMINO_J_1,            
            niketica::tetris::TETROMINO_J_2,            
            niketica::tetris::TETROMINO_J_3,
            niketica::tetris::TETROMINO_J_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_J);
        registry->emplace<niketica::tetris::Matrices3X3>(entity, matrices);
    }
    
    void TetrominoCreationSystem::createTetrominoL()
    {
        auto entity = createTetrominoBase();

        auto matrices = niketica::tetris::Matrices3X3
        {
            niketica::tetris::TETROMINO_L_1,            
            niketica::tetris::TETROMINO_L_2,            
            niketica::tetris::TETROMINO_L_3,
            niketica::tetris::TETROMINO_L_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_L);
        registry->emplace<niketica::tetris::Matrices3X3>(entity, matrices);
    }
    
    void TetrominoCreationSystem::createTetrominoO()
    {
        auto entity = createTetrominoBase();

        auto matrices = niketica::tetris::Matrices2X2
        {
            niketica::tetris::TETROMINO_O_1
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_O);
        registry->emplace<niketica::tetris::Matrices2X2>(entity, matrices);
    }
    
    void TetrominoCreationSystem::createTetrominoS()
    {
        auto entity = createTetrominoBase();

        auto matrices = niketica::tetris::Matrices3X3
        {
            niketica::tetris::TETROMINO_S_1,
            niketica::tetris::TETROMINO_S_2,
            niketica::tetris::TETROMINO_S_3,
            niketica::tetris::TETROMINO_S_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_S);
        registry->emplace<niketica::tetris::Matrices3X3>(entity, matrices);
    }
    
    void TetrominoCreationSystem::createTetrominoT()
    {
        auto entity = createTetrominoBase();

        auto matrices = niketica::tetris::Matrices3X3
        {
            niketica::tetris::TETROMINO_T_1,
            niketica::tetris::TETROMINO_T_2,
            niketica::tetris::TETROMINO_T_3,
            niketica::tetris::TETROMINO_T_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_T);
        registry->emplace<niketica::tetris::Matrices3X3>(entity, matrices);
    }

    void TetrominoCreationSystem::createTetrominoZ()
    {
        auto entity = createTetrominoBase();

        auto matrices = niketica::tetris::Matrices3X3
        {
            niketica::tetris::TETROMINO_Z_1,
            niketica::tetris::TETROMINO_Z_2,
            niketica::tetris::TETROMINO_Z_3,
            niketica::tetris::TETROMINO_Z_4
        };
        registry->emplace<niketica::component::Color>(entity, niketica::tetris::COLOR_Z);
        registry->emplace<niketica::tetris::Matrices3X3>(entity, matrices);
    }
    
    entt::entity TetrominoCreationSystem::createTetrominoBase()
    {
        auto entity = registry->create();
        registry->emplace<niketica::tetris::Tetromino>(entity);
        registry->emplace<niketica::tetris::BlockPositions>(entity);
        registry->emplace<niketica::tetris::GridPosition>(entity, niketica::tetris::GridPosition{{4,20}});
        registry->emplace<niketica::tetris::SetBlockPositions>(registry->create());
        return entity;
    }

    void TetrominoCreationSystem::destroyTetromino()
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino>();
        for (auto entity : viewTetromino)
        {
            registry->destroy(entity);
        }
    }
    
}
