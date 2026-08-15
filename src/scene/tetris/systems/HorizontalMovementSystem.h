#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"
#include "scene/tetris/components/TetrisComponents.h"
#include "scene/tetris/util/TetrisUtil.h"

namespace niketica::tetris
{

    class HorizontalMovementSystem : public niketica::systems::ISystem
    {
    public:
        HorizontalMovementSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override;
        void render() override {};

    private:
        void moveTetrominoHorizontal();
        bool canMoveLeft();
        bool canMoveRight();

    };

}
