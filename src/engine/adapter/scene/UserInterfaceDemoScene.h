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
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/SystemContext.h"

namespace niketica::scene
{
    class UserInterfaceDemoScene : public IScene
    {
    public:
        UserInterfaceDemoScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~UserInterfaceDemoScene() = default;

        void input() override;
        void update(float deltaTime) override;
        void render() override;
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        void init();

    };
}