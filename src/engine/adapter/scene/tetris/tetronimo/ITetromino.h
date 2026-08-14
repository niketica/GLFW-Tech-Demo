#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "engine/adapter/scene/tetris/tetronimo/Tetrominoes.h"

namespace niketica::tetris
{

    using MATRIX_2X2 = char[2][2];
    using MATRIX_3X3 = char[3][3];
    using MATRIX_4X4 = char[4][4];

    enum class Rotation
    {
        _1,
        _2,
        _3,
        _4
    };

    struct ITetromino
    {
    public:
        virtual ~ITetromino() = default;

        const glm::vec4& getColor() const
        {
            return color;
        }

        const std::vector<glm::ivec2>& getBlockPositions() const
        {
            return blockPositions;
        }

        void rotate()
        {
            switch (rotation)
            {
            case Rotation::_1:
                rotation = Rotation::_2;
                break;
            case Rotation::_2:
                rotation = Rotation::_3;
                break;
            case Rotation::_3:
                rotation = Rotation::_4;
                break;
            case Rotation::_4:
                rotation = Rotation::_1;
                break;
            }
            setBlockPositions();
        }

    protected:
        Rotation rotation = Rotation::_1;
        std::vector<glm::ivec2> blockPositions;
        glm::vec4 color;

        virtual void setBlockPositions() = 0;
        virtual const MATRIX_2X2& getMatrix2x2() const = 0;
        virtual const MATRIX_3X3& getMatrix3x3() const = 0;
        virtual const MATRIX_4X4& getMatrix4x4() const = 0;

        void setBlockPositions2x2()
        {
            blockPositions.clear();
            const auto& matrix = getMatrix2x2();
            for (int y=0; y<2; y++)
            {
                for (int x=0; x<2; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,y});
                    }
                }
            }
        }

        void setBlockPositions3x3()
        {
            blockPositions.clear();
            const auto& matrix = getMatrix3x3();
            for (int y=0; y<3; y++)
            {
                for (int x=0; x<3; x++)
                {
                    if (matrix[y][x] == 'X')
                    {
                        blockPositions.emplace_back(glm::ivec2{x,y});
                    }
                }
            }
        }

        void setBlockPositions4x4()
        {
            blockPositions.clear();
            const auto& matrix = getMatrix4x4();
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
        }
    };

}
