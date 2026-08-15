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
#include "scene/tetris/tetronimo/Tetrominoes.h"
#include "scene/tetris/components/TetrisComponents.h"
#include "scene/tetris/systems/VerticalMovementSystem.h"

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
        entt::entity createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill);
        entt::entity createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        entt::entity getEntityAtGridPosition(int x, int y);

        void clearGrid();

        void setBlockPositions();
        niketica::tetris::MATRIX_2X2& getMatrix2x2(entt::entity entity);
        niketica::tetris::MATRIX_3X3& getMatrix3x3(entt::entity entity);
        niketica::tetris::MATRIX_4X4& getMatrix4x4(entt::entity entity);
        void rotateTetromino();

        void createRandomTetromino();
        void destroyTetromino();
        entt::entity createTetrominoBase();
        void createTetrominoI();
        void createTetrominoJ();
        void createTetrominoL();
        void createTetrominoO();
        void createTetrominoS();
        void createTetrominoT();
        void createTetrominoZ();

        void moveTetrominoHorizontal();

        void colorTetrominoOnGrid();
        bool canMoveLeft();
        bool canMoveRight();
        bool shouldPushLeft();
        bool shouldPushRight();

        void moveTetrominoBlocksToGrid();

        niketica::tetris::GameState& getGameState();

        entt::entity getEntityAtGridPositionTEMP(int x, int y);

    };
}
