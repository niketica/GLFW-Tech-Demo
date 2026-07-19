#pragma once

#include <entt/entt.hpp>
#include <glad/glad.h>

#include "component/Color.h"
#include "component/Input.h"
#include "component/Sprite.h"
#include "component/Sound.h"
#include "component/Transform.h"
#include "component/TextureHandle.h"
#include "component/Window.h"
#include "engine/core/renderer/IRenderContext.h"
#include "systems/SystemRepository.h"

namespace niketica::scene
{
    class TestScene
    {
    public:
        TestScene(
            entt::registry* registry,
            niketica::systems::SystemRepository* systemRepository,
            niketica::renderer::IRenderContext* renderContext
        );
        ~TestScene() = default;

        void input();
        void update(float deltaTime);
        void render();
    
    private:
        entt::registry* registry;
        niketica::systems::SystemRepository* systemRepository;
        niketica::renderer::IRenderContext* renderContext;

        void init();

    };
}