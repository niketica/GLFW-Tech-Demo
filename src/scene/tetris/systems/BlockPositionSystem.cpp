#include "scene/tetris/systems/BlockPositionSystem.h"

namespace niketica::tetris
{

    void BlockPositionSystem::update(float dt)
    {
        bool componentFound = false;
        auto viewSetBlockPositions = registry->view<niketica::tetris::SetBlockPositions>();
        for (auto entity : viewSetBlockPositions)
        {
            componentFound = true;
            registry->destroy(entity);
        }

        if (componentFound)
        {
            setBlockPositions();
        }
    }

    void BlockPositionSystem::setBlockPositions()
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::BlockPositions>();
        auto entity = viewTetromino.front();

       auto& blockPositions = viewTetromino.get<niketica::tetris::BlockPositions>(entity).blockPositions;

        if (registry->any_of<niketica::tetris::Matrices2X2>(entity))
        {
            auto& matrix = getMatrix2x2(entity);
            blockPositions.clear();
            for (int y=0; y<2; y++)
            {
                for (int x=0; x<2; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,3-y});
                    }
                }
            }
        }
        else if (registry->any_of<niketica::tetris::Matrices3X3>(entity))
        {
            auto& matrix = getMatrix3x3(entity);
            blockPositions.clear();
            for (int y=0; y<3; y++)
            {
                for (int x=0; x<3; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,3-y});
                    }
                }
            }
        }
        else if (registry->any_of<niketica::tetris::Matrices4X4>(entity))
        {
            auto& matrix = getMatrix4x4(entity);
            blockPositions.clear();
            for (int y=0; y<4; y++)
            {
                for (int x=0; x<4; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,3-y});
                    }
                }
            }
        }
    }

    niketica::tetris::MATRIX_2X2& BlockPositionSystem::getMatrix2x2(entt::entity entity)
    {
        auto matrices = registry->get<niketica::tetris::Matrices2X2>(entity);
        return matrices.matrix;
    }

    niketica::tetris::MATRIX_3X3& BlockPositionSystem::getMatrix3x3(entt::entity entity)
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::BlockPositions>();
        const auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(entity);
        auto matrices = registry->get<niketica::tetris::Matrices3X3>(entity);

        switch (tetromino.rotation)
        {
        case niketica::tetris::Rotation::_1:
            return matrices.matrix1;
        case niketica::tetris::Rotation::_2:
            return matrices.matrix2;
        case niketica::tetris::Rotation::_3:
            return matrices.matrix3;
        case niketica::tetris::Rotation::_4:
            return matrices.matrix4;
        }
        throw std::invalid_argument("TetrisScene::getMatrix3x3 - No valid rotation for tetromino.");
    }

    niketica::tetris::MATRIX_4X4& BlockPositionSystem::getMatrix4x4(entt::entity entity)
    {
        auto viewTetromino = registry->view<niketica::tetris::Tetromino, niketica::tetris::BlockPositions>();
        const auto& tetromino = viewTetromino.get<niketica::tetris::Tetromino>(entity);
        auto matrices = registry->get<niketica::tetris::Matrices4X4>(entity);

        switch (tetromino.rotation)
        {
        case niketica::tetris::Rotation::_1:
            return matrices.matrix1;
        case niketica::tetris::Rotation::_2:
            return matrices.matrix2;
        case niketica::tetris::Rotation::_3:
            return matrices.matrix3;
        case niketica::tetris::Rotation::_4:
            return matrices.matrix4;
        }
        throw std::invalid_argument("TetrisScene::getMatrix4x4 - No valid rotation for tetromino.");
    }
    
}
