#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace niketica::tetris
{

    // I-Tetromino
    inline const char TETROMINO_I_1[4][4] =
    {
        { ' ', ' ', ' ', ' ' },
        { 'X', 'X', 'X', 'X' },
        { ' ', ' ', ' ', ' ' },
        { ' ', ' ', ' ', ' ' },
    };
    inline const char TETROMINO_I_2[4][4] =
    {
        { ' ', ' ', 'X', ' ' },
        { ' ', ' ', 'X', ' ' },
        { ' ', ' ', 'X', ' ' },
        { ' ', ' ', 'X', ' ' },
    };
    inline const char TETROMINO_I_3[4][4] =
    {
        { ' ', ' ', ' ', ' ' },
        { ' ', ' ', ' ', ' ' },
        { 'X', 'X', 'X', 'X' },
        { ' ', ' ', ' ', ' ' },
    };
    inline const char TETROMINO_I_4[4][4] =
    {
        { ' ', 'X', ' ', ' ' },
        { ' ', 'X', ' ', ' ' },
        { ' ', 'X', ' ', ' ' },
        { ' ', 'X', ' ', ' ' },
    };

    // J-Tetromino
    inline const char TETROMINO_J_1[3][3] =
    {
        { 'X', ' ', ' ' },
        { 'X', 'X', 'X' },
        { ' ', ' ', ' ' },
    };
    inline const char TETROMINO_J_2[3][3] =
    {
        { ' ', 'X', 'X' },
        { ' ', 'X', ' ' },
        { ' ', 'X', ' ' },
    };
    inline const char TETROMINO_J_3[3][3] =
    {
        { ' ', ' ', ' ' },
        { 'X', 'X', 'X' },
        { ' ', ' ', 'X' },
    };
    inline const char TETROMINO_J_4[3][3] =
    {
        { ' ', 'X', ' ' },
        { ' ', 'X', ' ' },
        { 'X', 'X', ' ' },
    };
    
    // L-Tetromino
    inline const char TETROMINO_L_1[3][3] =
    {
        { ' ', ' ', 'X' },
        { 'X', 'X', 'X' },
        { ' ', ' ', ' ' },
    };
    inline const char TETROMINO_L_2[3][3] =
    {
        { ' ', 'X', ' ' },
        { ' ', 'X', ' ' },
        { ' ', 'X', 'X' },
    };
    inline const char TETROMINO_L_3[3][3] =
    {
        { ' ', ' ', ' ' },
        { 'X', 'X', 'X' },
        { 'X', ' ', ' ' },
    };
    inline const char TETROMINO_L_4[3][3] =
    {
        { 'X', 'X', ' ' },
        { ' ', 'X', ' ' },
        { ' ', 'X', ' ' },
    };
    
    // O-Tetromino
    inline const char TETROMINO_O_1[2][2] =
    {
        { 'X', 'X' },
        { 'X', 'X' },
    };
    
    // S-Tetromino
    inline const char TETROMINO_S_1[3][3] =
    {
        { ' ', 'X', 'X' },
        { 'X', 'X', ' ' },
        { ' ', ' ', ' ' },
    };
    inline const char TETROMINO_S_2[3][3] =
    {
        { ' ', 'X', ' ' },
        { ' ', 'X', 'X' },
        { ' ', ' ', 'X' },
    };
    inline const char TETROMINO_S_3[3][3] =
    {
        { ' ', ' ', ' ' },
        { ' ', 'X', 'X' },
        { 'X', 'X', ' ' },
    };
    inline const char TETROMINO_S_4[3][3] =
    {
        { 'X', ' ', ' ' },
        { 'X', 'X', ' ' },
        { ' ', 'X', ' ' },
    };
    
    // T-Tetromino
    inline const char TETROMINO_T_1[3][3] =
    {
        { ' ', 'X', ' ' },
        { 'X', 'X', 'X' },
        { ' ', ' ', ' ' },
    };
    inline const char TETROMINO_T_2[3][3] =
    {
        { ' ', 'X', ' ' },
        { ' ', 'X', 'X' },
        { ' ', 'X', ' ' },
    };
    inline const char TETROMINO_T_3[3][3] =
    {
        { ' ', ' ', ' ' },
        { 'X', 'X', 'X' },
        { ' ', 'X', ' ' },
    };
    inline const char TETROMINO_T_4[3][3] =
    {
        { ' ', 'X', ' ' },
        { 'X', 'X', ' ' },
        { ' ', 'X', ' ' },
    };
    
    // Z-Tetromino
    inline const char TETROMINO_Z_1[3][3] =
    {
        { 'X', 'X', ' ' },
        { ' ', 'X', 'X' },
        { ' ', ' ', ' ' },
    };
    inline const char TETROMINO_Z_2[3][3] =
    {
        { ' ', ' ', 'X' },
        { ' ', 'X', 'X' },
        { ' ', 'X', ' ' },
    };
    inline const char TETROMINO_Z_3[3][3] =
    {
        { ' ', ' ', ' ' },
        { 'X', 'X', ' ' },
        { ' ', 'X', 'X' },
    };
    inline const char TETROMINO_Z_4[3][3] =
    {
        { ' ', 'X', ' ' },
        { 'X', 'X', ' ' },
        { 'X', ' ', ' ' },
    };

    enum class Rotation
    {
        _1,
        _2,
        _3,
        _4
    };

    // Structs
    struct ITetromino
    {
        Rotation rotation = Rotation::_1;

        virtual ~ITetromino() = default;
        virtual glm::vec4 getColor() = 0;
        virtual std::vector<glm::ivec2> getBlockPositions() = 0;

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
        }
    };

    struct Tetromino_I : public ITetromino
    {
        using Matrix = char[4][4];

        glm::vec4 getColor() override
        {
            return { 1.0f, 0.0f, 0.0f, 1.0f };
        }

        const Matrix& getMatrix() const
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