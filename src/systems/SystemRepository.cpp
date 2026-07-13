#include "systems/SystemRepository.h"

namespace niketica::systems
{
    SystemRepository::SystemRepository(entt::registry* registry, const niketica::engine::InputState& state, const niketica::engine::InputMap& map)
        : registry(registry), state(state), map(map)
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
    }

}
