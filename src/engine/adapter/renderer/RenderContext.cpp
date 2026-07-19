#include "engine/adapter/renderer/RenderContext.h"

namespace niketica::renderer
{

    void RenderContext::init()
    {
        initWindow();
        initRenderers();
    }

    void RenderContext::initWindow()
    {
        std::cout << "INFO::RenderContext::initWindow -     Initializing Window...";
        if (!glfwInit())
        {
            std::cerr << "ERROR::RenderContext::initWindow - Cannot initialize GLFW" << std::endl;
            throw std::invalid_argument( "ERROR::RenderContext::initWindow - Cannot initialize GLFW" );
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
            std::cerr << "ERROR::RenderContext::initWindow - Cannot create GLFW window" << std::endl;
            throw std::invalid_argument( "ERROR::RenderContext::initWindow - Cannot create GLFW window" );
        }

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "ERROR::RenderContext::initWindow - Cannot initialize GLAD" << std::endl;
            throw std::invalid_argument( "ERROR::RenderContext::initWindow - Cannot initialize GLAD" );
        }
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        
        glfwSwapInterval(0); // turn off vsync

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        std::cout << "DONE!" << std::endl;
    }

    void RenderContext::initRenderers()
    {
        std::cout << "INFO::RenderContext::initRenderers -     Initializing Renderers...";

        textureLoader = std::make_unique<TextureLoader>();
        spriteInstancedRenderer = std::make_unique<SpriteInstancedRenderer>(textureLoader.get());
        textRenderer = std::make_unique<TextRenderer>(1920.0f, 1080.0f); // Just give the dimensions hard coded for now

        std::cout << "DONE!" << std::endl;     
    }
    
    bool RenderContext::windowShouldClose()
    {
        return glfwWindowShouldClose(window);
    };

    void RenderContext::startFrame()
    {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    };

    void RenderContext::endFrame()
    {
        glfwSwapBuffers(window);
        glfwPollEvents();        
    };

    void RenderContext::setWindowTitle(const char* title)
    {
        glfwSetWindowTitle(window, title);
    };

}
