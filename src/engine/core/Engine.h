#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/ISceneContext.h"

namespace niketica::engine
{
    class Engine
    {
    public:
        Engine(std::unique_ptr<EngineServices> engineServices, std::unique_ptr<niketica::scene::ISceneContext> sceneContext);
        ~Engine() = default;

        void start();

    private:

        const unsigned int SCR_WIDTH = 1920;
        const unsigned int SCR_HEIGHT = 1080;
        float windowWidth = static_cast<float>(SCR_WIDTH);
        float windowHeight = static_cast<float>(SCR_HEIGHT);

        float xOffset = 0.0;
        float yOffset = 0.0;

        float xPos = 0.0f;
        float yPos = 0.0f;

        float row = 2.0f;
        float col = 2.0f;
        
        bool running = false;

        glm::vec3 clearColor = glm::vec3(0.2f, 0.3f, 0.3f);

        std::unique_ptr<EngineServices> engineServices;
        std::unique_ptr<entt::registry> registry;
        std::unique_ptr<niketica::scene::ISceneContext> sceneContext;

        void init();
        void loop();

        void input();
        void update(float deltaTime);
        void render();

        void initSystems();
        void initInput();

    };

}
