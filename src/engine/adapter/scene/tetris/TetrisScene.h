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
#include "engine/adapter/systems/SystemContext.h"
#include "engine/adapter/scene/tetris/ITetromino.h"
#include "engine/adapter/scene/tetris/TetrominoI.h"

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

        enum class Direction
        {
            UNDEFINED,
            UP,
            DOWN,
            LEFT,
            RIGHT
        };

        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        const static int gridWidth = 10;
        const static int gridHeight = 20;
        float blockSize = 54.0f;
        std::vector<entt::entity> gridEntities;

        Direction currentDirection = Direction::RIGHT;

        float currentTimer = 0.0f;
        float cooldownPeriod = 0.2f;

        bool gameActive = false;

        std::unique_ptr<tetris::ITetromino> currentTetromino;

        void init();
        entt::entity createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill);
        entt::entity createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        entt::entity getEntityAtGridPosition(int x, int y);

        void clearGrid();

    };
}
