#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine/core/renderer/IRenderContext.h"
#include "engine/adapter/renderer/TextureLoader.h"
#include "engine/adapter/renderer/SpriteInstancedRenderer.h"
#include "engine/adapter/renderer/NineSliceInstancedRenderer.h"
#include "engine/adapter/renderer/text/TextRenderer.h"

namespace niketica::renderer
{

    class RenderContext : public IRenderContext
    {
    public:
        const unsigned int SCR_WIDTH = 1920;
        const unsigned int SCR_HEIGHT = 1080;
        
        RenderContext() = default;
        ~RenderContext() = default;

        void init() override;
        bool windowShouldClose() override;
        void startFrame() override;
        void endFrame() override;
        void setWindowTitle(const char* title) override;

        GLFWwindow* getWindow()
        {
            return window;
        }

        void reset() override;

        void setWindowMode(component::Window& windowComponent, component::WindowMode mode) override;
        void setWindowSize(component::Window& windowComponent, int width, int height) override;
        void updateViewport(component::Viewport& viewportComponent, const component::Window& windowComponent, const component::Camera& camera) override;
        void updateCamera(component::Camera& camera) override;

    private:
        GLFWwindow* window;
        glm::vec3 clearColor = glm::vec3(0.2f, 0.3f, 0.3f);

        void initWindow();
        void initRenderers();
        void releaseRenderers();

    };

}
