#pragma once

#include <memory>
#include <entt/entt.hpp>
#include <glad/glad.h>

#include "component/Color.h"
#include "component/Input.h"
#include "component/Sprite.h"
#include "component/Sound.h"
#include "component/Transform.h"
#include "component/TextureHandle.h"
#include "component/Window.h"
#include "engine/core/EngineServices.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/SystemContext.h"

namespace niketica::scene
{
    class TestScene
    {
    public:
        TestScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~TestScene() = default;

        void input();
        void update(float deltaTime);
        void render();
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        void init();

    };
}