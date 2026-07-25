#include "engine/adapter/systems/SystemContext.h"

namespace niketica::systems
{

    void SystemContext::init()
    {
        systems.emplace_back(std::make_unique<niketica::systems::InputSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::SoundSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::SpriteRendererSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::TextRendererSystem>(registry, engineServices));
    }

}
