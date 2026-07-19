#include "systems/SystemRepository.h"

namespace niketica::systems
{
    SystemRepository::SystemRepository(
        entt::registry* registry,
        const niketica::input::InputState* state,
        const niketica::input::InputMap* map,
        niketica::sound::ISoundContext* soundContext
    )
        : registry(registry), state(state), map(map), soundContext(soundContext)
    {
        initSystems();
    }

    void SystemRepository::reset()
    {
        inputSystem.reset();

        initSystems();
    }

    void SystemRepository::initSystems()
    {
        inputSystem = std::make_unique<InputSystem>(state, map, registry);
        soundSystem = std::make_unique<SoundSystem>(registry, soundContext);
    }

}
