#pragma once

#include <memory>
#include <glm/glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "engine/core/input/InputState.h"
#include "engine/core/input/IInputContext.h"

namespace niketica::input
{
    class InputBackendGLFW : public IInputContext
    {
    public:
        InputBackendGLFW(GLFWwindow* window)
        {
            std::cout << "INFO::InputBackendGLFW::InputBackendGLFW - Initializing input..." << std::endl;
            this->inputState = std::make_unique<InputState>();

            std::cout << "INFO::InputBackendGLFW::InputBackendGLFW -     Initializing callback functions" << std::endl;
            glfwSetWindowUserPointer(window, this);
            glfwSetKeyCallback(window, keyCallback);
            glfwSetCursorPosCallback(window, mouseCallback);
            glfwSetMouseButtonCallback(window, mouse_button_callback);
            glfwSetScrollCallback(window, scrollCallback);
            
            this->inputMap = std::make_unique<InputMap>();

            std::cout << "INFO::InputBackendGLFW::InputBackendGLFW -     Populating input map" << std::endl;
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

            std::cout << "INFO::InputBackendGLFW::InputBackendGLFW - Done initializing input!" << std::endl;
        }

        void clearState() override
        {
            inputState->clearState();
        }

    private:
        glm::vec2 lastMousePos{ 0.0f };
        bool firstMouse = true;

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto* backend = static_cast<InputBackendGLFW*>(glfwGetWindowUserPointer(window));
            auto& button = backend->inputState->key(key);

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
            backend->inputState->mousePos = current;
            backend->inputState->mouseDelta += current - backend->lastMousePos;
            backend->lastMousePos = current;
        }

        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
        {
            auto* backend = static_cast<InputBackendGLFW*>(glfwGetWindowUserPointer(window));
            backend->inputState->scrollDelta += static_cast<float>(yoffset);
        }

        static void mouse_button_callback(GLFWwindow* window, int mouseButton, int action, int mods)
        {
            auto* backend = static_cast<InputBackendGLFW*>(glfwGetWindowUserPointer(window));
            auto& button = backend->inputState->key(mouseButton);

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
