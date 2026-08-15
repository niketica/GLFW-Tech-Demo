#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/systems/ISystemContext.h"
#include "systems/common/WorldTransformSystem.h"
#include "systems/sound/SoundSystem.h"
#include "systems/input/InputSystem.h"
#include "systems/renderer/SpriteRendererSystem.h"
#include "systems/renderer/TextRendererSystem.h"
#include "systems/renderer/ResolutionSystem.h"
#include "systems/renderer/RectangleRenderSystem.h"
#include "systems/userinterface/UILayoutSystem.h"
#include "systems/userinterface/UINavigationSystem.h"

namespace niketica::systems
{

    class SystemContext : public ISystemContext
    {
    public:
        SystemContext(entt::registry* registry, niketica::engine::EngineServices* engineServices) : ISystemContext(registry, engineServices) {}
        ~SystemContext() = default;

        void init() override;

    };

}
