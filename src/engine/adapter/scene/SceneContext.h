#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/ISceneContext.h"
#include "engine/core/scene/IScene.h"
#include "engine/adapter/scene/MainMenuScene.h"
#include "engine/adapter/scene/SnakeScene.h"
#include "engine/adapter/scene/TetrisScene.h"

namespace niketica::scene
{
    class SceneContext : public ISceneContext
    {
    public:
        SceneContext(
            niketica::engine::EngineServices* engineServices
        );
        ~SceneContext() = default;

        void input() override;
        void update(float dt) override;
        void render() override;
        void initScenes() override;

    private:
        niketica::engine::EngineServices* engineServices;
        std::unordered_map<niketica::component::SceneType, std::unique_ptr<IScene>> sceneMap;
        IScene* currentScene = nullptr;

        void setCurrentScene(niketica::component::SceneType type);

    };
}