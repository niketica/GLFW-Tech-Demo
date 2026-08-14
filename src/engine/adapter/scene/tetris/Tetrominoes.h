#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace niketica::tetris
{

    using MATRIX_2X2 = char[2][2];
    using MATRIX_3X3 = char[3][3];
    using MATRIX_4_4 = char[4][4];

    // I-Tetromino
    inline const glm::vec4 COLOR_I = { 1.0f, 0.0f, 0.0f, 1.0f };
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
    inline const glm::vec4 COLOR_J = { 1.0f, 0.0f, 0.0f, 1.0f };
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
    inline const glm::vec4 COLOR_L = { 1.0f, 0.0f, 0.0f, 1.0f };
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
    inline const glm::vec4 COLOR_O = { 1.0f, 0.0f, 0.0f, 1.0f };
    inline const char TETROMINO_O_1[2][2] =
    {
        { 'X', 'X' },
        { 'X', 'X' },
    };
    
    // S-Tetromino
    inline const glm::vec4 COLOR_S = { 1.0f, 0.0f, 0.0f, 1.0f };
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
    inline const glm::vec4 COLOR_T = { 1.0f, 0.0f, 0.0f, 1.0f };
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
    inline const glm::vec4 COLOR_Z = { 1.0f, 0.0f, 0.0f, 1.0f };
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

}