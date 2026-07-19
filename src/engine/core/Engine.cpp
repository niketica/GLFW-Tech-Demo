#include "engine/core/Engine.h"

namespace niketica::engine
{

    Engine::Engine(
            std::unique_ptr<niketica::asset::IAssetReader> assetReader,
            std::unique_ptr<niketica::renderer::IRenderContext> renderContext,
            std::unique_ptr<niketica::input::IInputContext> inputContext
        ) : assetReader(std::move(assetReader)), renderContext(std::move(renderContext)), inputContext(std::move(inputContext))
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

        std::cout << "INFO::Engine::init -     Initializing temporary data...";
        
        float w = windowWidth;
        float h = windowHeight;

        auto windowComponent = niketica::component::Window();
        windowComponent.x = 100;
        windowComponent.y = 100;
        windowComponent.width = (float)SCR_WIDTH;
        windowComponent.height = (float)SCR_HEIGHT;
        windowComponent.projection = glm::ortho(
            0.f, windowWidth,
            0.f, windowHeight,
            -10.f, 10.f
        );
        windowComponent.scale = 1.0f;
        windowComponent.view = glm::mat4(1.0f);

        registry->emplace<niketica::component::Window>(registry->create(), windowComponent);

        std::cout << "DONE!" << std::endl;

        std::cout << "INFO::Engine::init - Engine initialized." << std::endl;
    }

    void Engine::initSystems()
    {
        std::cout << "INFO::Engine::init -     Initializing internal systems..." << std::endl;

        std::cout << "INFO::Engine::init -     Initializing sound..." << std::endl;
        soundBackend = std::make_unique<niketica::sound::SoundBackendMiniaudio>();
        soundBackend->init();

        std::cout << "INFO::Engine::init -     Initializing ECS systems..." << std::endl;
        systemRepository = std::make_unique<niketica::systems::SystemRepository>(registry.get(), inputContext->getInputState(), inputContext->getInputMap(), soundBackend.get());

        std::cout << "INFO::Engine::init -     Initializing scenes..." << std::endl;
        sceneRepository = std::make_unique<niketica::scene::SceneRepository>(registry.get(), systemRepository.get(), renderContext.get());

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

        running = true;
        while (running)
        {
            currentTime = static_cast<float>(glfwGetTime());
            deltaTime = currentTime - lastTime;
            lastTime = currentTime;

            accumulator += deltaTime;
            updates = 0;

            while (accumulator >= fixedDeltaTime && updates < maxUpdatesPerFrame)
            {
                if (renderContext->windowShouldClose())
                {
                    std::cout << "INFO::Engine::loop - Window close requested, exiting loop." << std::endl;
                    running = false;
                    break;
                }
                
                input(fixedDeltaTime);
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
                renderContext->setWindowTitle(title.c_str());
            }
        }

        std::cout << "INFO::Engine::loop - Main loop ended." << std::endl;
        glfwTerminate();
    }

    void Engine::input(float deltaTime)
    {
        systemRepository->getInputSystem()->update();
        inputContext->clearState();
        
        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::ESCAPE].pressed)
        {
            std::cout << "INFO::Engine::update - Escape key pressed, exiting loop." << std::endl;
            running = false;
            return;
        }

        sceneRepository->getTestScene()->input();
    }

    void Engine::update(float deltaTime)
    {

        sceneRepository->getTestScene()->update(deltaTime);
    }

    void Engine::render()
    {
        renderContext->startFrame();
        sceneRepository->getTestScene()->render();
        renderContext->endFrame();
    }
}
