#include "systems/SystemRepository.h"

namespace niketica::systems
{
    SystemRepository::SystemRepository(
        entt::registry* registry,
        const niketica::input::InputState* state,
        const niketica::input::InputMap* map,
        niketica::sound::SoundBackendMiniaudio* soundBackend
    )
        : registry(registry), state(state), map(map), soundBackend(soundBackend)
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
        soundSystem = std::make_unique<SoundSystem>(registry, soundBackend);
    }

}
