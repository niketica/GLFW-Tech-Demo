#pragma once

#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "engine/input/InputState.h"

namespace niketica::engine
{
    class InputBackendGLFW
    {
    public:
        explicit InputBackendGLFW(GLFWwindow* window, InputState& state)
            : state(state)
        {
            glfwSetWindowUserPointer(window, this);

            glfwSetKeyCallback(window, keyCallback);
            glfwSetCursorPosCallback(window, mouseCallback);
            glfwSetMouseButtonCallback(window, mouse_button_callback);
            glfwSetScrollCallback(window, scrollCallback);
        }

        void clearState()
        {
            state.clearState();
        }

    private:
        engine::InputState& state;
        glm::vec2 lastMousePos{ 0.0f };
        bool firstMouse = true;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto* backend = static_cast<InputBackendGLFW*>(glfwGetWindowUserPointer(window));
            auto& button = backend->state.key(key);

            if (action == GLFW_PRESS)
            {
                if (!button.down)
                {
                    button.pressed = true;
                }
                button.down = true;
            }
            else if (action == GLFW_RELEASE)
            {
                button.down = false;
                button.released = true;
            }
        }

        static void mouseCallback(GLFWwindow* window, double xpos, double ypos)
        {
            auto* backend = static_cast<InputBackendGLFW*>(glfwGetWindowUserPointer(window));

            if (backend->firstMouse)
            {
                backend->lastMousePos = { xpos, ypos };
                backend->firstMouse = false;
            }

            glm::vec2 current{ xpos, ypos };
            backend->state.mousePos = current;
            backend->state.mouseDelta += current - backend->lastMousePos;
            backend->lastMousePos = current;
        }

        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
        {
            auto* backend = static_cast<InputBackendGLFW*>(glfwGetWindowUserPointer(window));
            backend->state.scrollDelta += static_cast<float>(yoffset);
        }

        static void mouse_button_callback(GLFWwindow* window, int mouseButton, int action, int mods)
        {
            auto* backend = static_cast<InputBackendGLFW*>(glfwGetWindowUserPointer(window));
            auto& button = backend->state.key(mouseButton);

            if (action == GLFW_PRESS)
            {
                if (!button.down)
                {
                    button.pressed = true;
                }
                button.down = true;
            }
            else if (action == GLFW_RELEASE)
            {
                button.down = false;
                button.released = true;
            }
        }
    };

}
