#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace niketica::tetris
{

    using MATRIX_2X2 = const char[2][2];
    using MATRIX_3X3 = const char[3][3];
    using MATRIX_4X4 = const char[4][4];

    enum class Rotation
    {
        _1,
        _2,
        _3,
        _4
    };

    enum class Direction
    {
        UNDEFINED,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct Tetromino
    {
        Rotation rotation = Rotation::_1;
        Direction direction = Direction::UNDEFINED;
    };

    struct BlockPositions
    {
        std::vector<glm::ivec2> blockPositions;
    };

    struct Matrices2X2
    {
        MATRIX_2X2& matrix; // The O Tetromino does not rotate so only 1 matrix is sufficient.
    };

    struct Matrices3X3
    {
        MATRIX_3X3& matrix1;
        MATRIX_3X3& matrix2;
        MATRIX_3X3& matrix3;
        MATRIX_3X3& matrix4;
    };

    struct Matrices4X4
    {
        MATRIX_4X4& matrix1;
        MATRIX_4X4& matrix2;
        MATRIX_4X4& matrix3;
        MATRIX_4X4& matrix4;
    };

    struct GridPosition
    {
        glm::ivec2 position;
    };
    
    struct SolidBlock {};

    struct GameState
    {
        const static int gridWidth = 10;
        const static int gridHeight = 20;
        const float blockSize = 54.0f;
        Direction currentDirection = Direction::RIGHT;
        float currentTimer = 0.0f;
        float cooldownPeriod = 0.6f;
        bool gameActive = false;
        bool verticalHitDetected = false;
    };

    struct GridBlock {};

    struct SetBlockPositions {};
    struct CreateRandomTetromino {};
    struct DestroyTetromino {};

}
