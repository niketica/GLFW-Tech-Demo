#pragma once

#include <entt/entt.hpp>

#include "component/Sound.h"
#include "engine/core/sound/ISoundContext.h"

namespace niketica::systems
{

    class SoundSystem
    {
    public:
        SoundSystem(entt::registry* registry, niketica::sound::ISoundContext* soundContext)
            : registry(registry), soundContext(soundContext) {}
        ~SoundSystem() = default;

        void update(float deltaTime);

    private:
        entt::registry* registry;
        niketica::sound::ISoundContext* soundContext;
    };

}
