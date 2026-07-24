#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/ISceneContext.h"
#include "engine/core/scene/IScene.h"
#include "engine/adapter/scene/TestScene.h"
#include "engine/adapter/scene/UserInterfaceDemoScene.h"

namespace niketica::scene
{
    class SceneContext : public ISceneContext
    {
    public:
        SceneContext(
            niketica::engine::EngineServices* engineServices
        );
        ~SceneContext() = default;

        void input() override { currentScene->input(); };
        void update(float dt) override { currentScene->update(dt); };
        void render() override { currentScene->render(); };

        void initScenes() override;

    private:
        niketica::engine::EngineServices* engineServices;
        std::unordered_map<niketica::component::SceneType, std::unique_ptr<IScene>> sceneMap;
        IScene* currentScene = nullptr;

        void setCurrentScene(niketica::component::SceneType type)
        {
            auto it = sceneMap.find(type);
            if (it != sceneMap.end())
            {
                currentScene = it->second.get();
            }
            else
            {
                std::cerr << "ERROR::SceneContext::setCurrentScene - Unknown scene type" << std::endl;
            }
        }

    };
}