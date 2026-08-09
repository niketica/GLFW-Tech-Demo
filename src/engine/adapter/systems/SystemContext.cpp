#include "engine/adapter/systems/SystemContext.h"

namespace niketica::systems
{

    void SystemContext::init()
    {
        systems.emplace_back(std::make_unique<niketica::systems::ResolutionSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::WorldTransformSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::InputSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::SoundSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::SpriteRendererSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::TextRendererSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::UILayoutSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::UINavigationSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::RectangleRenderSystem>(registry, engineServices));
    }

}
