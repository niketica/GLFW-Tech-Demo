#include "engine/core/Engine.h"

namespace niketica::engine
{

    Engine::Engine(
            std::unique_ptr<EngineServices> engineServices,
            std::unique_ptr<niketica::scene::ISceneContext> sceneContext
        ) : engineServices(std::move(engineServices)), sceneContext(std::move(sceneContext))
    {}

    void Engine::start()
    {
        init();
        loop();
    }

    void Engine::init()
    {
        std::cout << "INFO::Engine::init - Start engine initialization." << std::endl;
        registry = std::make_unique<entt::registry>();
        initInput();
        initSystems();

        std::cout << "INFO::Engine::init - Initializing temporary data..." << std::endl;
        
        float w = windowWidth;
        float h = windowHeight;

        auto window = niketica::component::Window();
        window.width = (float)SCR_WIDTH;
        window.height = (float)SCR_HEIGHT;

        auto viewport = niketica::component::Viewport();
        viewport.x = 100;
        viewport.y = 100;
        viewport.width = (float)SCR_WIDTH;
        viewport.height = (float)SCR_HEIGHT;
        viewport.scale = 1.0f;

        auto windowEntity = registry->create();
        registry->emplace<niketica::component::Window>(windowEntity, window);
        registry->emplace<niketica::component::Viewport>(windowEntity, viewport);
        registry->emplace<niketica::component::Persistent>(windowEntity);
        
        auto camera = niketica::component::Camera();
        camera.projection = glm::ortho(
            0.f, windowWidth,
            0.f, windowHeight,
            -10.f, 10.f
        );
        camera.view = glm::mat4(1.0f);
        camera.zoom = 1.0f;
        camera.virtualWidth = (float)SCR_WIDTH;
        camera.virtualHeight = (float)SCR_HEIGHT;

        auto cameraEntity = registry->create();
        registry->emplace<niketica::component::Camera>(cameraEntity, camera);
        registry->emplace<niketica::component::ActiveCamera>(cameraEntity);
        registry->emplace<niketica::component::Persistent>(cameraEntity);
        
        sceneContext->setRegistry(registry.get());
        sceneContext->initScenes();

        registry->emplace<niketica::component::EngineConfig>(registry->create());

        std::cout << "INFO::Engine::init - Engine initialized." << std::endl;
    }

    void Engine::initSystems()
    {
        std::cout << "INFO::Engine::init -     Initializing internal systems..." << std::endl;

        std::cout << "INFO::Engine::init -     Initializing scenes..." << std::endl;

        std::cout << "INFO::Engine::init -     Done initializing internal systems." << std::endl;
    }

    void Engine::initInput()
    {
        auto input = registry->create();
        registry->emplace<niketica::component::InputComponent>(input);
        registry->emplace<niketica::component::InputRepeatConfig>(input);
        registry->emplace<niketica::component::Persistent>(input);
    }

    void Engine::loop()
    {
        std::cout << "INFO::Engine::loop - Main loop started." << std::endl;
        float currentTime = static_cast<float>(glfwGetTime());
        float lastTime = currentTime;
        float deltaTime = 0.0f;
        const float fixedDeltaTime = 1.0f / 60.0f;
        float accumulator = 0.0f;
        const unsigned int maxUpdatesPerFrame = 5;
        unsigned int updates = 0;

        float fpsTime = 0.0f;
        unsigned int frameCount = 0;
        unsigned int fps = 0;

        auto viewEngineConfig = registry->view<niketica::component::EngineConfig>();
        auto& engineConfig = viewEngineConfig.get<niketica::component::EngineConfig>(viewEngineConfig.front());
        engineConfig.running = true;
        while (engineConfig.running)
        {
            currentTime = static_cast<float>(glfwGetTime());
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            accumulator += deltaTime;
            updates = 0;

            while (accumulator >= fixedDeltaTime && updates < maxUpdatesPerFrame)
            {
                if (engineServices->getRenderContext()->windowShouldClose())
                {
                    std::cout << "INFO::Engine::loop - Window close requested, exiting loop." << std::endl;
                    auto viewEngineConfig = registry->view<niketica::component::EngineConfig>();
                    auto& engineConfig = viewEngineConfig.get<niketica::component::EngineConfig>(viewEngineConfig.front());
                    engineConfig.running = false;
                    break;
                }
                
                input();
                update(fixedDeltaTime);
                accumulator -= fixedDeltaTime;
                updates++;
            }

            render();

            fpsTime += deltaTime;
            frameCount++;
            if (fpsTime >= 1.0f)
            {
                fps = frameCount;
                frameCount = 0;
                fpsTime = 0.0f;

                std::string title("GLFW Tech Demo - FPS: ");
                title += std::to_string(fps);
                engineServices->getRenderContext()->setWindowTitle(title.c_str());
            }
        }

        std::cout << "INFO::Engine::loop - Main loop ended." << std::endl;
        glfwTerminate();
    }

    void Engine::input()
    {        
        sceneContext->input();
    }

    void Engine::update(float deltaTime)
    {

        sceneContext->update(deltaTime);
    }

    void Engine::render()
    {
        engineServices->getRenderContext()->startFrame();
        sceneContext->render();
        engineServices->getRenderContext()->endFrame();
    }
}
