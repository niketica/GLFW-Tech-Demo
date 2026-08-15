#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"
#include "scene/tetris/components/TetrisComponents.h"
#include "scene/tetris/util/TetrisUtil.h"

namespace niketica::tetris
{

    class GridSystem : public niketica::systems::ISystem
    {
    public:
        GridSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override;
        void render() override {};

    private:
        void createGrid();
        entt::entity createRectangleWithBorder(const glm::vec3& position, const glm::vec2& size, const glm::vec4& fillColor, const glm::vec4& borderColor, float borderThickness, float fill);
        entt::entity createRectangleBorderless(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

        void clearGrid();
        void colorTetrominoOnGrid();
        void moveTetrominoBlocksToGrid();

        void clearGridLines();
        void clearGridLine(int y);
        void moveUpperGridLinesDown(int y);

    };

}
