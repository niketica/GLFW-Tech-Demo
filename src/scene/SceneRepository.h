#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/renderer/RendererRepository.h"
#include "systems/SystemRepository.h"
#include "scene/TestScene.h"

namespace niketica::scene
{
    class SceneRepository
    {
    public:
        SceneRepository(
            entt::registry* registry,
            niketica::systems::SystemRepository* systemRepository,
            niketica::renderer::RendererRepository* rendererRepository
        );
        ~SceneRepository() = default;

        TestScene* getTestScene() const { return testScene.get(); }

    private:
        entt::registry* registry;
        niketica::systems::SystemRepository* systemRepository;
        niketica::renderer::RendererRepository* rendererRepository;

        std::unique_ptr<TestScene> testScene;

        void initScenes();

    };
}