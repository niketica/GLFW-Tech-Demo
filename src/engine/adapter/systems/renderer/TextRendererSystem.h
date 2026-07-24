#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{

    class TextRendererSystem : public ISystem
    {
    public:
        TextRendererSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}

        void input() override {}
        void update(float dt) override {}
        void render() override;

    private:
        const std::unordered_map<niketica::component::Action, niketica::input::ActionBinding>& mapBindings() const
        {
            return engineServices->getInputContext()->getInputMap()->getBindings();
        }

    };

}
