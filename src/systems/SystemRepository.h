#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/core/input/InputState.h"
#include "engine/core/input/InputMap.h"
#include "engine/core/sound/ISoundContext.h"
#include "systems/input/InputSystem.h"
#include "systems/sound/SoundSystem.h"

namespace niketica::systems
{
    class SystemRepository
    {
        public:
            SystemRepository(
                entt::registry* registry,
                const niketica::input::InputState* state,
                const niketica::input::InputMap* map,
                niketica::sound::ISoundContext* soundContext
            );
            ~SystemRepository() = default;

            void reset();

            InputSystem* getInputSystem() const { return inputSystem.get(); }
            SoundSystem* getSoundSystem() const { return soundSystem.get(); }

        private:
            entt::registry* registry;
            const niketica::input::InputState* state;
            const niketica::input::InputMap* map;
            niketica::sound::ISoundContext* soundContext;

            std::unique_ptr<InputSystem> inputSystem;
            std::unique_ptr<SoundSystem> soundSystem;

            void initSystems();
    };

}
