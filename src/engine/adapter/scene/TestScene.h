#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/SystemContext.h"

namespace niketica::scene
{
    class TestScene : public IScene
    {
    public:
        TestScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~TestScene() = default;

        void input() override;
        void update(float deltaTime) override;
        void render() override;
        void reset() override;
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        void init();

    };
}