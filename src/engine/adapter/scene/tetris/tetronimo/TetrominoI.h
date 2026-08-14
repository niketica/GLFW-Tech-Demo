#pragma once

#include "engine/adapter/scene/tetris/tetronimo/ITetromino.h"

namespace niketica::tetris
{

    struct Tetromino_I : public ITetromino
    {
    public:
        Tetromino_I()
        {
            color = COLOR_I;
            setBlockPositions();
        }
    
    private:
        const MATRIX_2X2& getMatrix2x2() const override
        {
            throw std::invalid_argument("Tetromino::getMatrix2x2 - Not a valid matrix dimension.");

        }

        const MATRIX_3X3& getMatrix3x3() const override
        {
            throw std::invalid_argument("Tetromino::getMatrix3x3 - Not a valid matrix dimension.");
        }

        const MATRIX_4X4& getMatrix4x4() const override
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
            throw std::invalid_argument("Tetromino::getMatrix4x4 - No valid rotation.");
        }

        void setBlockPositions() override
        {
            setBlockPositions4x4();
        }

    };

}
