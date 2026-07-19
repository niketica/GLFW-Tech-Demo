#pragma once

#include <entt/entt.hpp>

#include "engine/adapter/sound/SoundBackendMiniaudio.h"
#include "component/Sound.h"

namespace niketica::systems
{

    class SoundSystem
    {
    public:
        SoundSystem(entt::registry* registry, niketica::sound::SoundBackendMiniaudio* soundBackend)
            : registry(registry), soundBackend(soundBackend) {}
        ~SoundSystem() = default;

        void update(float deltaTime);

    private:
        entt::registry* registry;
        niketica::sound::SoundBackendMiniaudio* soundBackend;
    };

}
