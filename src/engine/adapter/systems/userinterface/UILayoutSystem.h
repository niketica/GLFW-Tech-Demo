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

        void input() override {}
        void update(float dt) override;
        void render() override {};

    private:
        void updatePositionContainer(entt::entity container, const niketica::component::Window& window);
        void updateLayoutContainer(entt::entity container);
        void updateLayoutChildElements(entt::entity container);

    };

}
