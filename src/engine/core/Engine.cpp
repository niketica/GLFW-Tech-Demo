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
        
        float w = niketica::config::ORIGINAL_WIDTH;
        float h = niketica::config::ORIGINAL_HEIGHT;

        auto window = niketica::component::Window();
        window.width = (int)w;
        window.height = (int)h;

        auto viewport = niketica::component::Viewport();
        viewport.x = 100;
        viewport.y = 100;
        viewport.width = (int)w;
        viewport.height = (int)h;
        viewport.scale = 1.0f;

        niketica::component::RenderSettings renderSettings;
        renderSettings.worldReferenceResolution = { w, h };
        renderSettings.uiReferenceResolution = { w, h };

        auto windowEntity = registry->create();
        registry->emplace<niketica::component::Persistent>(windowEntity);
        registry->emplace<niketica::component::Window>(windowEntity, window);
        registry->emplace<niketica::component::Viewport>(windowEntity, viewport);
        registry->emplace<niketica::component::RenderSettings>(windowEntity, renderSettings);
        
        niketica::component::Camera camera;
        camera.projection = glm::ortho(
            0.f, renderSettings.worldReferenceResolution.x,
            0.f, renderSettings.worldReferenceResolution.y,
            -10.f, 10.f
        );
        camera.view = glm::mat4(1.0f);
        camera.zoom = 1.0f;

        auto cameraEntity = registry->create();
        registry->emplace<niketica::component::Persistent>(cameraEntity);
        registry->emplace<niketica::component::Camera>(cameraEntity, camera);
        registry->emplace<niketica::component::ActiveCamera>(cameraEntity);

        auto engineConfigEntity = registry->create();
        registry->emplace<niketica::component::Persistent>(engineConfigEntity);
        registry->emplace<niketica::component::EngineConfig>(engineConfigEntity);
        
        auto userInterface = registry->create();
        registry->emplace<niketica::component::Persistent>(userInterface);
        registry->emplace<niketica::component::UIFocus>(userInterface);
        
        sceneContext->setRegistry(registry.get());
        sceneContext->initScenes();

        updateViewport();

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

    void Engine::updateViewport()
    {
        auto viewViewport = registry->view<niketica::component::Viewport>();
        auto& viewport = viewViewport.get<niketica::component::Viewport>(viewViewport.front());
        auto viewRenderSettings = registry->view<niketica::component::RenderSettings>();
        auto viewWindow = registry->view<niketica::component::Window>();
        const auto& window = viewWindow.get<niketica::component::Window>(viewWindow.front());
        const auto& renderSettings = viewRenderSettings.get<niketica::component::RenderSettings>(viewRenderSettings.front());
        engineServices->getRenderContext()->updateViewport(viewport, window, renderSettings);
    }
}
