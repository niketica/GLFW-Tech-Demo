#pragma once

#include <entt/entt.hpp>

#include "component/Sound.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{

    class SoundSystem : public ISystem
    {
    public:
        SoundSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}
        ~SoundSystem() = default;

        void input() override {};
        void update(float dt) override;
        void render() override {};

    };

}
