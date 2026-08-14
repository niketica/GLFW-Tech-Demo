#pragma once

#include "engine/adapter/scene/tetris/tetronimo/ITetromino.h"

namespace niketica::tetris
{

    struct Tetromino_O : public ITetromino
    {
    public:
        Tetromino_O()
        {
            color = COLOR_O;
            setBlockPositions();
        }
    
    private:
        const MATRIX_2X2& getMatrix2x2() const override
        {
            return TETROMINO_O_1;
        }

        const MATRIX_3X3& getMatrix3x3() const override
        {
            throw std::invalid_argument("Tetromino::getMatrix3x3 - Not a valid matrix dimension.");
        }

        const MATRIX_4X4& getMatrix4x4() const override
        {
            throw std::invalid_argument("Tetromino::getMatrix4x4 - Not a valid matrix dimension.");
        }

        void setBlockPositions() override
        {
            setBlockPositions2x2();
        }

    };

}
