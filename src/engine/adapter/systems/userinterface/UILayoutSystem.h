#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{

    class UILayoutSystem : public ISystem
    {
    public:
        UILayoutSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}
        ~UILayoutSystem() = default;

        void input() override {}
        void update(float dt) override;
        void render() override {};

    private:
        void updatePositionContainer(entt::entity container, const niketica::component::Window& window);
        void updateLayoutContainer(entt::entity container);

        // Layout Vertical 
        void updateLayoutVertical(entt::entity container);
        float measureTotalHeight(const niketica::component::UIChildren& children, const float spacing);
        float computeStartY(const float containerSizeY, const float totalHeight, const niketica::component::UIContentPadding padding, const niketica::component::AlignmentVertical alignment) const;
        void placeChildrenVertically(const niketica::component::UIChildren& children, const float spacing, float cursorY);
        void alignChildrenHorizontally(const niketica::component::UIChildren& children, const float containerPosZ, const float containerSizeX, const niketica::component::UIContentPadding padding);

        // Vertical
        void updateLayoutHorizontal(entt::entity container);
        float measureTotalWidth(const niketica::component::UIChildren& children, const float spacing);
        float computeStartX(const float containerSizeX, const float totalWidth, const niketica::component::UIContentPadding padding, const niketica::component::AlignmentHorizontal alignment) const;
        void placeChildrenHorizontally(const niketica::component::UIChildren& children, const float spacing, float cursorX);
        void alignChildrenVertically(const niketica::component::UIChildren& children, const float containerPosZ, const float containerSizeY, const niketica::component::UIContentPadding padding);

    };

}
