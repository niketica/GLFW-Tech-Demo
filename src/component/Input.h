#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace niketica::component
{
    struct ButtonState
    {
        bool down = false; // currently held
        bool pressed = false; // went down THIS frame
        bool released = false; // went up THIS frame
    };

    enum class Action
    {
        ARROW_UP,
        ARROW_DOWN,
        ARROW_LEFT,
        ARROW_RIGHT,
        ENTER,
        NP_ENTER,
        ESCAPE,
        SPACE,
        LEFT_SHIFT,
        RIGHT_SHIFT,
        A,B,C,D,E,F,G,
        H,I,J,K,L,M,N,O,P,
        Q,R,S,T,U,V,W,X,Y,Z,
        _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,
        MOUSE_LEFT,
        MOUSE_RIGHT,
        MOUSE_MIDDLE,
        MOUSE_SCROLL_UP,
        MOUSE_SCROLL_DOWN,
        TILDE,
        MINUS,
        BRACKET_OPEN,
        BRACKET_CLOSE
    };

    struct ActionRepeat
    {
        float timer = 0.0f;
        bool repeating = false;
    };

    struct InputComponent
    {
        std::unordered_map<Action, ButtonState> actions;
        std::unordered_map<Action, ActionRepeat> repeats;
        glm::vec2 mousePos{ 0.0f };
        glm::vec2 mouseDelta{ 0.0f };
        float scrollDelta{ 0.0f };
    };

    struct InputRepeatConfig
    {
        float initialDelay = 0.30f; // delay before repeat starts
        float repeatRate = 0.10f; // seconds between repeats
    };

    struct InputContextArea
    {
        glm::vec2 position{ 0.0f, 0.0f };
        glm::vec2 size{ 0.0f, 0.0f };
    };

    struct InputContextEntry
    {
        bool active = false;
        int priority = 0; // higher priority contexts override lower ones
        std::vector<InputContextArea> areas; // if empty, context is active everywhere
    };

    struct InputContextConfig
    {
        std::vector<entt::entity> contexts;
        bool dirty = false;
    };

    struct InputContext
    {
        entt::entity panel = entt::null;
    };

}
