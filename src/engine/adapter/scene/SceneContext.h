#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "engine/core/EngineServices.h"
#include "engine/core/scene/ISceneContext.h"
#include "engine/adapter/scene/TestScene.h"

namespace niketica::scene
{
    class SceneContext : public ISceneContext
    {
    public:
        SceneContext(
            niketica::engine::EngineServices* engineServices
        );
        ~SceneContext() = default;

        void input() override { testScene->input(); };
        void update(float dt) override { testScene->update(dt); };
        void render() override { testScene->render(); };

        void initScenes() override;

    private:
        niketica::engine::EngineServices* engineServices;

        std::unique_ptr<TestScene> testScene;

    };
}