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
#include "engine/adapter/scene/tetris/tetronimo/Tetrominoes.h"

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

        void init();
        entt::entity createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill);
        entt::entity createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        entt::entity getEntityAtGridPosition(int x, int y);

        void clearGrid();

        void setBlockPositions();
        MATRIX_2X2& getMatrix2x2(entt::entity entity);
        MATRIX_3X3& getMatrix3x3(entt::entity entity);
        MATRIX_4X4& getMatrix4x4(entt::entity entity);
        void rotateTetromino();

        entt::entity createTetrominoBase();
        void createTetrominoI();
        void createTetrominoJ();
        void createTetrominoL();
        void createTetrominoO();
        void createTetrominoS();
        void createTetrominoT();
        void createTetrominoZ();

        void colorTetrominoOnGrid();

    };
}
