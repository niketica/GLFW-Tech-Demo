#include "systems/userinterface/UINavigationSystem.h"

namespace niketica::systems
{

    void UINavigationSystem::update(float dt)
    {
        auto inputView = registry->view<niketica::component::InputComponent, niketica::component::InputRepeatConfig>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());
        const auto& repeatConfig = inputView.get<niketica::component::InputRepeatConfig>(inputView.front());

        // There should always be exactly one UIFocus component.
        auto viewFocus = registry->view<niketica::component::UIFocus>();
        auto& focus = viewFocus.get<niketica::component::UIFocus>(viewFocus.front());

        bool focusChanged = false;

        if (input.mousePos == lastMousePos)
        {
            focusChanged = handleKeyboardNavigation
            (
                dt,
                input,
                repeatConfig,
                focus
            );
        }
        else
        {
            focusChanged = true;
            bool mouseInsideButton = handleMouseHover(input, focus);
            if (!mouseInsideButton)
            {
                focus.index = -1;
            }
        }

        if (focusChanged)
        {
            updateFocusedVisuals(focus);
        }

        if (focus.index >= 0)
        {
            handleActivation(input, focus);
        }

    }

    bool UINavigationSystem::handleKeyboardNavigation
    (
        float deltaTime,
        component::InputComponent& input,
        const component::InputRepeatConfig& repeatConfig,
        component::UIFocus& focus
    )
    {
        int previousIndex = focus.index;

        if (moveMenuUp(deltaTime, input, repeatConfig))
        {
            focus.index = (focus.index + focus.focusables.size() - 1) % focus.focusables.size();
            playMoveSound();
        }
        else if (moveMenuDown(deltaTime, input, repeatConfig))
        {
            focus.index = (focus.index + 1) % focus.focusables.size();
            playMoveSound();
        }

        return previousIndex != focus.index;
    }

    bool UINavigationSystem::handleMouseHover
    (
        component::InputComponent& input,
        component::UIFocus& focus
    )
    {
        bool mouseHighlightButton = false;
        lastMousePos = input.mousePos;

        int previousIndex = focus.index;
        for (size_t i = 0; i < focus.focusables.size(); ++i)
        {
            auto entity = focus.focusables.at(i);
            const auto& transform = registry->get<component::Transform>(entity);

            if (isMouseInsideButton(input.mousePos, transform))
            {
                mouseHighlightButton = true;
                focus.index = static_cast<int>(i);

                if (previousIndex != focus.index)
                {
                    playMoveSound();
                    return mouseHighlightButton;
                }

                break;
            }
        }

        return mouseHighlightButton;
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
        const auto virtualMouse = niketica::util::screen::screenToVirtual(mousePos, viewport, renderSettings);

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
        const component::UIFocus& focus
    )
    {
        updateFocusedVisualsNineSlice(focus);
        updateFocusedVisualsContainer(focus);
    }
    
    void UINavigationSystem::updateFocusedVisualsNineSlice
    (
        const component::UIFocus& focus
    )
    {
        for (size_t i = 0; i < focus.focusables.size(); ++i)
        {
            auto entity = focus.focusables.at(i);

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
    
    void UINavigationSystem::updateFocusedVisualsContainer(const component::UIFocus& focus)
    {
        for (size_t i = 0; i < focus.focusables.size(); ++i)
        {
            auto entity = focus.focusables.at(i);
            updateFocusedVisualsContainer(i == focus.index, entity);
        }
    }
    
    void UINavigationSystem::updateFocusedVisualsContainer(const bool focus, entt::entity entity)
    {
        if (registry->all_of<niketica::component::UINormalColor, niketica::component::UIHighlightColor>(entity))
        {
            if (focus)
            {
                const auto& highlightColor = registry->get<niketica::component::UIHighlightColor>(entity).color;
                
                if (registry->all_of<niketica::component::Color>(entity))
                {
                    auto& color = registry->get<niketica::component::Color>(entity).value;
                    color = highlightColor;
                }
                if (registry->all_of<niketica::component::Text>(entity))
                {
                    auto& text = registry->get<niketica::component::Text>(entity);
                    text.color = highlightColor;
                }
            }
            else
            {
                const auto& normalColor = registry->get<niketica::component::UINormalColor>(entity).color;
                
                if (registry->all_of<niketica::component::Color>(entity))
                {
                    auto& color = registry->get<niketica::component::Color>(entity).value;
                    color = normalColor;
                }
                if (registry->all_of<niketica::component::Text>(entity))
                {
                    auto& text = registry->get<niketica::component::Text>(entity);
                    text.color = normalColor;
                }
            }
        }

        if (registry->all_of<niketica::component::UIChildren>(entity))
        {
            const auto& children = registry->get<niketica::component::UIChildren>(entity).children;
            for (auto child : children)
            {
                updateFocusedVisualsContainer(focus, child);
            }
        }
    }

    void UINavigationSystem::handleActivation
    (
        component::InputComponent& input,
        component::UIFocus& focus
    )
    {
        entt::entity activated = entt::null;

        if (menuItemConfirmed(input))
        {
            activated = focus.focusables.at(focus.index);
        }
        else if (input.actions[component::Action::MOUSE_LEFT].released)
        {
            activated = findClickedButton(input.mousePos, focus.focusables);

            if (activated != entt::null)
            {
                focus.index = findButtonIndex(activated, focus.focusables);
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
