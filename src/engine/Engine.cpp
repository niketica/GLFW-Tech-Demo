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
        std::cout << "INFO::Engine::init -     Initializing Window...";
        if (!glfwInit())
        {
            std::cerr << "ERROR::Engine::init - Cannot initialize GLFW" << std::endl;
            throw std::invalid_argument( "Cannot initialize GLFW" );
        }

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
        
        glfwSwapInterval(0); // turn off vsync
        std::cout << "DONE!" << std::endl;

        std::cout << "INFO::Engine::init -     Initializing internal systems...";

        pakReader = std::make_unique<niketica::asset::PakReader>(niketica::asset::COMPRESSION_PASSPHRASE);
        niketica::asset::AssetManager::Get().RegisterLoader<niketica::asset::File>(std::make_shared<niketica::asset::FileLoader>(pakReader.get()));

        basicShader = std::make_shared<niketica::renderer::Shader>("shaders/basic_shader.vert", "shaders/basic_shader.frag");

        std::cout << "DONE!" << std::endl;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        std::cout << "INFO::Engine::init - Engine initialized." << std::endl;
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
        // TODO
    }

    void Engine::update(float deltaTime)
    {
        // TODO
    }

    void Engine::render()
    {
        /* Render here */
        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        basicShader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}
