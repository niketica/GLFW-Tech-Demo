#include "engine/Engine.h"

namespace niketica::engine
{
    void Engine::start()
    {
        init();
        loop();
    }

    void Engine::init()
    {
        std::cout << "INFO::Engine::init - Start engine initialization." << std::endl;
        registry = std::make_unique<entt::registry>();
        initWindow();
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

    void Engine::initWindow()
    {
        std::cout << "INFO::Engine::init -     Initializing Window...";
        if (!glfwInit())
        {
            std::cerr << "ERROR::Engine::init - Cannot initialize GLFW" << std::endl;
            throw std::invalid_argument( "Cannot initialize GLFW" );
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW Tech Demo", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            std::cerr << "ERROR::Engine::init - Cannot create GLFW window" << std::endl;
            throw std::invalid_argument( "Cannot create GLFW window" );
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "ERROR::Engine::init - Cannot initialize GLAD" << std::endl;
            throw std::invalid_argument( "Cannot initialize GLAD" );
        }
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        
        glfwSwapInterval(0); // turn off vsync

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        std::cout << "DONE!" << std::endl;
    }

    void Engine::initSystems()
    {
        std::cout << "INFO::Engine::init -     Initializing internal systems..." << std::endl;

        pakReader = std::make_unique<niketica::asset::PakReader>(niketica::asset::COMPRESSION_PASSPHRASE);
        niketica::asset::AssetManager::Get().RegisterLoader<niketica::asset::File>(std::make_shared<niketica::asset::FileLoader>(pakReader.get()));
        systemRepository = std::make_unique<niketica::systems::SystemRepository>(registry.get(), *inputState, *inputMap);
        rendererRepository = std::make_unique<niketica::renderer::RendererRepository>();

        sceneRepository = std::make_unique<niketica::scene::SceneRepository>(registry.get(), systemRepository.get(), rendererRepository.get());

        std::cout << "INFO::Engine::init -     Done initializing internal systems." << std::endl;
    }

    void Engine::initInput()
    {
        inputState = std::make_unique<InputState>();

        inputBackend = std::make_unique<InputBackendGLFW>(window, *inputState);

        inputMap = std::make_unique<InputMap>();
        // inputSystem = std::make_unique<systems::InputSystem>(*inputState, *inputMap, registry.get());

        inputMap->bind(niketica::component::Action::ARROW_UP, GLFW_KEY_UP);
        inputMap->bind(niketica::component::Action::ARROW_DOWN, GLFW_KEY_DOWN);
        inputMap->bind(niketica::component::Action::ARROW_LEFT, GLFW_KEY_LEFT);
        inputMap->bind(niketica::component::Action::ARROW_RIGHT, GLFW_KEY_RIGHT);
        inputMap->bind(niketica::component::Action::ENTER, GLFW_KEY_ENTER);
        inputMap->bind(niketica::component::Action::NP_ENTER, GLFW_KEY_KP_ENTER);
        inputMap->bind(niketica::component::Action::ESCAPE, GLFW_KEY_ESCAPE);
        inputMap->bind(niketica::component::Action::TILDE, GLFW_KEY_GRAVE_ACCENT);
        inputMap->bind(niketica::component::Action::SPACE, GLFW_KEY_SPACE);
        inputMap->bind(niketica::component::Action::LEFT_SHIFT, GLFW_KEY_LEFT_SHIFT);
        inputMap->bind(niketica::component::Action::RIGHT_SHIFT, GLFW_KEY_RIGHT_SHIFT);
        inputMap->bind(niketica::component::Action::MINUS, GLFW_KEY_MINUS);
        inputMap->bind(niketica::component::Action::BRACKET_OPEN, GLFW_KEY_LEFT_BRACKET);
        inputMap->bind(niketica::component::Action::BRACKET_CLOSE, GLFW_KEY_RIGHT_BRACKET);

        inputMap->bind(niketica::component::Action::A, GLFW_KEY_A);
        inputMap->bind(niketica::component::Action::B, GLFW_KEY_B);
        inputMap->bind(niketica::component::Action::C, GLFW_KEY_C);
        inputMap->bind(niketica::component::Action::D, GLFW_KEY_D);
        inputMap->bind(niketica::component::Action::E, GLFW_KEY_E);
        inputMap->bind(niketica::component::Action::F, GLFW_KEY_F);
        inputMap->bind(niketica::component::Action::G, GLFW_KEY_G);
        inputMap->bind(niketica::component::Action::H, GLFW_KEY_H);
        inputMap->bind(niketica::component::Action::I, GLFW_KEY_I);
        inputMap->bind(niketica::component::Action::J, GLFW_KEY_J);
        inputMap->bind(niketica::component::Action::K, GLFW_KEY_K);
        inputMap->bind(niketica::component::Action::L, GLFW_KEY_L);
        inputMap->bind(niketica::component::Action::M, GLFW_KEY_M);
        inputMap->bind(niketica::component::Action::N, GLFW_KEY_N);
        inputMap->bind(niketica::component::Action::O, GLFW_KEY_O);
        inputMap->bind(niketica::component::Action::P, GLFW_KEY_P);
        inputMap->bind(niketica::component::Action::Q, GLFW_KEY_Q);
        inputMap->bind(niketica::component::Action::R, GLFW_KEY_R);
        inputMap->bind(niketica::component::Action::S, GLFW_KEY_S);
        inputMap->bind(niketica::component::Action::T, GLFW_KEY_T);
        inputMap->bind(niketica::component::Action::U, GLFW_KEY_U);
        inputMap->bind(niketica::component::Action::V, GLFW_KEY_V);
        inputMap->bind(niketica::component::Action::W, GLFW_KEY_W);
        inputMap->bind(niketica::component::Action::X, GLFW_KEY_X);
        inputMap->bind(niketica::component::Action::Y, GLFW_KEY_Y);
        inputMap->bind(niketica::component::Action::Z, GLFW_KEY_Z);

        inputMap->bind(niketica::component::Action::_0, GLFW_KEY_0);
        inputMap->bind(niketica::component::Action::_1, GLFW_KEY_1);
        inputMap->bind(niketica::component::Action::_2, GLFW_KEY_2);
        inputMap->bind(niketica::component::Action::_3, GLFW_KEY_3);
        inputMap->bind(niketica::component::Action::_4, GLFW_KEY_4);
        inputMap->bind(niketica::component::Action::_5, GLFW_KEY_5);
        inputMap->bind(niketica::component::Action::_6, GLFW_KEY_6);
        inputMap->bind(niketica::component::Action::_7, GLFW_KEY_7);
        inputMap->bind(niketica::component::Action::_8, GLFW_KEY_8);
        inputMap->bind(niketica::component::Action::_9, GLFW_KEY_9);

        inputMap->bind(niketica::component::Action::MOUSE_LEFT, GLFW_MOUSE_BUTTON_LEFT);
        inputMap->bind(niketica::component::Action::MOUSE_RIGHT, GLFW_MOUSE_BUTTON_RIGHT);
        inputMap->bind(niketica::component::Action::MOUSE_MIDDLE, GLFW_MOUSE_BUTTON_MIDDLE);

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
                if (glfwWindowShouldClose(window))
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
                glfwSetWindowTitle(window, title.c_str());
            }
        }

        std::cout << "INFO::Engine::loop - Main loop ended." << std::endl;
        glfwTerminate();
    }

    void Engine::input(float deltaTime)
    {
        systemRepository->getInputSystem()->update();
        inputBackend->clearState();
        
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
        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sceneRepository->getTestScene()->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
