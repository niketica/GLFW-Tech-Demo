#include "systems/userinterface/UINavigationSystem.h"

namespace niketica::systems
{

    void UINavigationSystem::update(float dt)
    {
        auto inputView = registry->view<niketica::component::InputComponent, niketica::component::InputRepeatConfig>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());
        const auto& repeatConfig = inputView.get<niketica::component::InputRepeatConfig>(inputView.front());

        auto viewUIActive = registry->view<niketica::component::UIActive, niketica::component::UIChildren, niketica::component::UIFocus>();
        for (auto entity : viewUIActive)
        {
            const auto& children = registry->get<niketica::component::UIChildren>(entity).children;
            if (children.empty())
            {
                continue;
            }
            auto& focus = registry->get<niketica::component::UIFocus>(entity);

            bool focusChanged = false;

            focusChanged |= handleKeyboardNavigation(
                dt,
                input,
                repeatConfig,
                focus,
                children);

            focusChanged |= handleMouseHover(
                input,
                focus,
                children);

            handleActivation(input, focus, children);

            if (focusChanged)
            {
                updateFocusedVisuals(entity, focus, children);
            }
        }
    }

    bool UINavigationSystem::handleKeyboardNavigation
    (
        float deltaTime,
        component::InputComponent& input,
        const component::InputRepeatConfig& repeatConfig,
        component::UIFocus& focus,
        const std::vector<entt::entity>& children
    )
    {
        int previousIndex = focus.index;

        if (moveMenuUp(deltaTime, input, repeatConfig))
        {
            focus.index = (focus.index + children.size() - 1) % children.size();
            playMoveSound();
        }
        else if (moveMenuDown(deltaTime, input, repeatConfig))
        {
            focus.index = (focus.index + 1) % children.size();
            playMoveSound();
        }

        return previousIndex != focus.index;
    }

    bool UINavigationSystem::handleMouseHover
    (
        component::InputComponent& input,
        component::UIFocus& focus,
        const std::vector<entt::entity>& children
    )
    {
        if (input.mousePos == lastMousePos)
        {
            return false;
        }
        lastMousePos = input.mousePos;

        int previousIndex = focus.index;
        for (size_t i = 0; i < children.size(); ++i)
        {
            auto entity = children[i];
            auto& transform = registry->get<component::Transform>(entity);

            if (isMouseInsideButton(input.mousePos, transform))
            {
                focus.index = static_cast<int>(i);

                if (previousIndex != focus.index)
                {
                    playMoveSound();
                    return true;
                }

                break;
            }
        }

        return false;
    }

    bool UINavigationSystem::shouldFireRepeated
    (
        component::InputComponent& input,
        component::Action action,
        float dt,
        const component::InputRepeatConfig& config
    )
    {
        const auto& state = input.actions[action];
        auto& repeat = input.repeats[action];

        // First press: fire immediately
        if (state.pressed)
        {
            repeat.timer = 0.0f;
            repeat.repeating = false;
            return true;
        }

        // Key released: reset repeat state
        if (!state.down)
        {
            repeat.timer = 0.0f;
            repeat.repeating = false;
            return false;
        }

        // Held key
        repeat.timer += dt;

        if (!repeat.repeating)
        {
            if (repeat.timer >= config.initialDelay)
            {
                repeat.timer = 0.0f;
                repeat.repeating = true;
                return true;
            }
        }
        else
        {
            if (repeat.timer >= config.repeatRate)
            {
                repeat.timer = 0.0f;
                return true;
            }
        }

        return false;
    }

    bool UINavigationSystem::moveMenuUp(float deltaTime, component::InputComponent& input, const component::InputRepeatConfig& repeatConfig)
    {
        return shouldFireRepeated(input, component::Action::ARROW_UP, deltaTime, repeatConfig)
            || shouldFireRepeated(input, component::Action::W, deltaTime, repeatConfig);
    }

    bool UINavigationSystem::moveMenuDown(float deltaTime, component::InputComponent& input, const component::InputRepeatConfig& repeatConfig)
    {
        return shouldFireRepeated(input, component::Action::ARROW_DOWN, deltaTime, repeatConfig)
            || shouldFireRepeated(input, component::Action::S, deltaTime, repeatConfig);
    }

    bool UINavigationSystem::menuItemConfirmed(component::InputComponent& input)
    {
        return input.actions[component::Action::ENTER].pressed
            || input.actions[component::Action::NP_ENTER].pressed
            || input.actions[component::Action::E].pressed;
    }

    bool UINavigationSystem::isMouseInsideButton(const glm::vec2& mousePos, const component::Transform& buttonTransform)
    {
        const auto& buttonPos = buttonTransform.position;
        const auto& buttonSize = buttonTransform.size;

        auto viewRenderSettings = registry->view<niketica::component::RenderSettings>();
        const auto& renderSettings = viewRenderSettings.get<niketica::component::RenderSettings>(viewRenderSettings.front());
        auto viewViewport = registry->view<niketica::component::Viewport>();
        const auto& viewport = viewViewport.get<niketica::component::Viewport>(viewViewport.front());
        const auto virtualMouse = niketica::util::screenToVirtual(mousePos, viewport, renderSettings);

        return virtualMouse.x >= buttonPos.x
            && virtualMouse.x <= buttonPos.x + buttonSize.x
            && virtualMouse.y >= buttonPos.y
            && virtualMouse.y <= buttonPos.y + buttonSize.y;
    }

    void UINavigationSystem::playMoveSound()
    {
        registry->emplace<component::SoundEffect>(registry->create(),
            component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/397604__nightflame__menu-fx-01.wav" });
    }

    void UINavigationSystem::playConfirmSound()
    {
        registry->emplace<component::SoundEffect>(registry->create(),
            component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/422514__nightflame__menu-fx-03-normal.wav" });
    }
    
    void UINavigationSystem::updateFocusedVisuals
    (
        entt::entity panel,
        const component::UIFocus& focus,
        const std::vector<entt::entity>& children
    )
    {
        for (size_t i = 0; i < children.size(); ++i)
        {
            auto entity = children[i];

            if (!registry->all_of<niketica::component::NineSlice>(entity))
            {
                continue;
            }

            auto& nineSlice = registry->get<niketica::component::NineSlice>(entity);

            // if (registry->all_of<niketica::component::Toggle>(entity))
            // {
            //     // TODO check if needed
            //     continue;
            // }

            // if (registry->all_of<niketica::component::RadioButton>(entity))
            // {
            //     // TODO check if needed
            //     continue;
            // }

            // auto& style = registry->get<niketica::component::ButtonStyle>(panel);
            nineSlice = (i == focus.index) ? niketica::config::NINE_SLICE_BUTTON_FOCUSED : niketica::config::NINE_SLICE_BUTTON_NORMAL;
        }
    }

    void UINavigationSystem::handleActivation
    (
        component::InputComponent& input,
        component::UIFocus& focus,
        const std::vector<entt::entity>& children
    )
    {
        entt::entity activated = entt::null;

        if (menuItemConfirmed(input))
        {
            activated = children[focus.index];
        }
        else if (input.actions[component::Action::MOUSE_LEFT].released)
        {
            activated = findClickedButton(input.mousePos, children);

            if (activated != entt::null)
            {
                focus.index = findButtonIndex(activated, children);
            }
        }

        if (activated != entt::null)
        {
            activateButton(activated);
        }
    }

    int UINavigationSystem::findButtonIndex(entt::entity button, const std::vector<entt::entity>& children)
    {
        for (size_t i = 0; i < children.size(); ++i)
        {
            if (children[i] == button)
            {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    entt::entity UINavigationSystem::findClickedButton(const glm::vec2& mousePos, const std::vector<entt::entity>& children)
    {
        for (auto entity : children)
        {
            const auto& transform = registry->get<component::Transform>(entity);
            if (isMouseInsideButton(mousePos, transform))
            {
                return entity;
            }
        }

        return entt::null;
    }

    void UINavigationSystem::activateButton(entt::entity button)
    {
        registry->emplace<component::ButtonActivated>(button);

        // if (registry->all_of<component::UIToggle>(button))
        // {
        //     activateToggle(button);
        // }

        // if (registry->all_of<component::UIRadioButton>(button))
        // {
        //     activateRadioButton(button);
        // }

        playConfirmSound();
    }

}
