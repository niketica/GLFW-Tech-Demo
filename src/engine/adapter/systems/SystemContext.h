#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/sound/SoundSystem.h"
#include "engine/adapter/systems/input/InputSystem.h"

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
