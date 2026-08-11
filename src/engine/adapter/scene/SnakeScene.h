#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/SystemContext.h"

namespace niketica::scene
{

    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class SnakeScene : public IScene
    {
    public:
        SnakeScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~SnakeScene() = default;

        void input() override;
        void update(float dt) override;
        void render() override;
        void reset() override;
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        const static int gridWidth = 17;
        const static int gridHeight = 17;
        std::vector<entt::entity> gridEntities;

        glm::ivec2 snakeHeadPosition{ 5, 5 };
        std::vector<glm::ivec2> snakeBodyPositions;
        Direction currentDirection = Direction::RIGHT;

        float currentTimer = 0.0f;
        float cooldownPeriod = 0.2f;

        void init();
        entt::entity createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill);
        entt::entity createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        entt::entity getEntityAtGridPosition(int x, int y);

        void clearGrid();
        void colorSnake();
        void moveSnake(float dt);
        void moveSnakeHead();
        void moveSnakeBody();

    };
}
