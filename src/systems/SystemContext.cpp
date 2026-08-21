#include "systems/SystemContext.h"

namespace niketica::systems
{

    void SystemContext::init()
    {
        /**
         * !!! Order of the systems vector dictates the order of all function calls of the systems !!!
         */
        systems.emplace_back(std::make_unique<niketica::systems::ResolutionSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::WorldTransformSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::InputSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::SoundSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::UILayoutSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::UINavigationSystem>(registry, engineServices));

        /**
         * Render systems
         * !!! The order here dictates the order of rendering !!!
         * This affects all rendering on the same z-level and blending with background colors.
         * You generally want to render text last for the glyphs to properly blend with the background.
         */
        systems.emplace_back(std::make_unique<niketica::systems::SpriteRendererSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::RectangleRenderSystem>(registry, engineServices));
        systems.emplace_back(std::make_unique<niketica::systems::TextRendererSystem>(registry, engineServices));
    }

}
