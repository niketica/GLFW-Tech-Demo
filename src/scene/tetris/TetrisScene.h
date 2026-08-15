#pragma once

#include <vector>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "systems/SystemContext.h"
#include "scene/tetris/components/Tetrominoes.h"
#include "scene/tetris/components/TetrisComponents.h"
#include "scene/tetris/systems/TetrominoCreationSystem.h"
#include "scene/tetris/systems/BlockPositionSystem.h"
#include "scene/tetris/systems/HorizontalMovementSystem.h"
#include "scene/tetris/systems/VerticalMovementSystem.h"
#include "scene/tetris/systems/RotationSystem.h"
#include "scene/tetris/systems/GridSystem.h"
#include "scene/tetris/util/TetrisUtil.h"

namespace niketica::scene
{

    class TetrisScene : public IScene
    {
    public:
        TetrisScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~TetrisScene() = default;

        void input() override;
        void update(float dt) override;
        void render() override;
        void reset() override;
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        void init();

    };
}
