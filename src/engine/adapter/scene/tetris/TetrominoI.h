#pragma once

#include "engine/adapter/scene/tetris/ITetromino.h"

namespace niketica::tetris
{

    struct Tetromino_I : public ITetromino
    {
        const glm::vec4& getColor() const override
        {
            return COLOR_I;
        }

        const MATRIX_4_4& getMatrix() const
        {
            switch (rotation)
            {
            case Rotation::_1:
            {
                return TETROMINO_I_1;
            }
                break;
            case Rotation::_2:
            {
                return TETROMINO_I_2;                
            }
                break;
            case Rotation::_3:
            {
                return TETROMINO_I_3;
                
            }
                break;
            case Rotation::_4:
            {
                return TETROMINO_I_4;                
            }
                break;
            }
            throw std::invalid_argument("Tetromino::getMatrix - No valid rotation.");
        }

        std::vector<glm::ivec2> getBlockPositions() override
        {
            const auto& matrix = getMatrix();

            std::vector<glm::ivec2> blockPositions;
            for (int y=0; y<4; y++)
            {
                for (int x=0; x<4; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,y});
                    }
                }
            }

            return blockPositions;
        }
    };

}
