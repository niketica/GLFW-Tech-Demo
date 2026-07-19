#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/adapter/input/InputState.h"
#include "engine/adapter/input/InputMap.h"
#include "engine/adapter/sound/SoundBackendMiniaudio.h"
#include "systems/input/InputSystem.h"
#include "systems/sound/SoundSystem.h"

namespace niketica::systems
{
    class SystemRepository
    {
        public:
            SystemRepository(
                entt::registry* registry,
                const niketica::engine::InputState& state,
                const niketica::engine::InputMap& map,
                niketica::sound::SoundBackendMiniaudio* soundBackend
            );
            ~SystemRepository() = default;

            void reset();

            InputSystem* getInputSystem() const { return inputSystem.get(); }
            SoundSystem* getSoundSystem() const { return soundSystem.get(); }

        private:
            entt::registry* registry;
            const niketica::engine::InputState& state;
            const niketica::engine::InputMap& map;
            niketica::sound::SoundBackendMiniaudio* soundBackend;

            std::unique_ptr<InputSystem> inputSystem;
            std::unique_ptr<SoundSystem> soundSystem;

            void initSystems();
    };

}
