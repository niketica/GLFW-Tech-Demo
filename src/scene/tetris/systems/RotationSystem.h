#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"
#include "scene/tetris/components/TetrisComponents.h"

namespace niketica::tetris
{

    class RotationSystem : public niketica::systems::ISystem
    {
    public:
        RotationSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override;
        void render() override {};

    private:
        void rotateTetromino();

    };

}
