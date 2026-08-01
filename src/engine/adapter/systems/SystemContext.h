#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/common/WorldTransformSystem.h"
#include "engine/adapter/systems/sound/SoundSystem.h"
#include "engine/adapter/systems/input/InputSystem.h"
#include "engine/adapter/systems/renderer/SpriteRendererSystem.h"
#include "engine/adapter/systems/renderer/TextRendererSystem.h"
#include "engine/adapter/systems/renderer/ResolutionSystem.h"
#include "engine/adapter/systems/userinterface/UILayoutSystem.h"
#include "engine/adapter/systems/userinterface/UINavigationSystem.h"

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
