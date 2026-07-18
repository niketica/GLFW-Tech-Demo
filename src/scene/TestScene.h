#pragma once

#include <entt/entt.hpp>

#include "component/Input.h"
#include "component/Sprite.h"
#include "component/Sound.h"
#include "component/Transform.h"
#include "component/TextureHandle.h"
#include "component/Window.h"
#include "engine/renderer/RendererRepository.h"
#include "systems/SystemRepository.h"

namespace niketica::scene
{
    class TestScene
    {
    public:
        TestScene(
            entt::registry* registry,
            niketica::systems::SystemRepository* systemRepository,
            niketica::renderer::RendererRepository* rendererRepository
        );
        ~TestScene() = default;

        void input();
        void update(float deltaTime);
        void render();
    
    private:
        entt::registry* registry;
        niketica::systems::SystemRepository* systemRepository;
        niketica::renderer::RendererRepository* rendererRepository;

        void init();

    };
}