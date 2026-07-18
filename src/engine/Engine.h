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

#include "component/Common.h"
#include "component/Input.h"
#include "engine/asset/AssetManager.h"
#include "engine/asset/PakReader.h"
#include "engine/asset/FileLoader.h"
#include "engine/renderer/RendererRepository.h"
#include "engine/input/InputState.h"
#include "engine/input/InputBackendGLFW.h"
#include "engine/input/InputMap.h"
#include "systems/SystemRepository.h"
#include "scene/SceneRepository.h"

namespace niketica::engine
{
    class Engine
    {
    public:
        Engine() = default;
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

        GLFWwindow* window;
        glm::vec3 clearColor = glm::vec3(0.2f, 0.3f, 0.3f);

        std::unique_ptr<niketica::asset::PakReader> pakReader;
        std::unique_ptr<niketica::renderer::RendererRepository> rendererRepository;

        std::unique_ptr<entt::registry> registry;
        std::unique_ptr<InputState> inputState;
        std::unique_ptr<InputBackendGLFW> inputBackend;
        std::unique_ptr<InputMap> inputMap;
        std::unique_ptr<niketica::systems::SystemRepository> systemRepository;
        std::unique_ptr<niketica::scene::SceneRepository> sceneRepository;

        void init();
        void loop();

        void input(float deltaTime);
        void update(float deltaTime);
        void render();

        void initWindow();
        void initSystems();
        void initInput();

    };

}
