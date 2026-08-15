#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"
#include "scene/tetris/components/TetrisComponents.h"

namespace niketica::tetris
{

    class BlockPositionSystem : public niketica::systems::ISystem
    {
    public:
        BlockPositionSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override;
        void render() override {};

    private:
        
        void setBlockPositions();
        niketica::tetris::MATRIX_2X2& getMatrix2x2(entt::entity entity);
        niketica::tetris::MATRIX_3X3& getMatrix3x3(entt::entity entity);
        niketica::tetris::MATRIX_4X4& getMatrix4x4(entt::entity entity);

    };

}
