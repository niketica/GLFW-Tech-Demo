#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"
#include "scene/TestScene.h"

namespace niketica::scene
{
    class SceneRepository
    {
    public:
        SceneRepository(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~SceneRepository() = default;

        TestScene* getTestScene() const { return testScene.get(); }

    private:
        entt::registry* registry;
        niketica::engine::EngineServices* engineServices;

        std::unique_ptr<TestScene> testScene;

        void initScenes();

    };
}