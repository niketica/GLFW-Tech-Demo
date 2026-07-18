#include "systems/SystemRepository.h"

namespace niketica::systems
{
    SystemRepository::SystemRepository(
        entt::registry* registry,
        const niketica::engine::InputState& state,
        const niketica::engine::InputMap& map,
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
