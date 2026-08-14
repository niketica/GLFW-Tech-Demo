#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "engine/adapter/scene/tetris/Tetrominoes.h"

namespace niketica::tetris
{

    enum class Rotation
    {
        _1,
        _2,
        _3,
        _4
    };

    struct ITetromino
    {
        Rotation rotation = Rotation::_1;

        virtual ~ITetromino() = default;
        virtual const glm::vec4& getColor() const = 0;
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

}
