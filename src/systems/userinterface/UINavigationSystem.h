#pragma once

#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/config/Config.h"
#include "engine/util/ScreenUtil.h"
#include "engine/core/systems/ISystem.h"
#include "engine/core/EngineServices.h"

namespace niketica::systems
{

    class UINavigationSystem : public ISystem
    {
    public:
        UINavigationSystem(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : ISystem(registry, engineServices) {}
        ~UINavigationSystem() = default;

        void input() override {};
        void update(float dt) override;
        void render() override {};

    private:
        glm::vec2 lastMousePos = { 0.0f, 0.0f };

        bool handleKeyboardNavigation
        (
            float deltaTime,
            component::InputComponent& input,
            const component::InputRepeatConfig& repeatConfig,
            component::UIFocus& focus
        );

        bool handleMouseHover
        (
            component::InputComponent& input,
            component::UIFocus& focus
        );
        
        bool shouldFireRepeated
        (
            component::InputComponent& input,
            component::Action action,
            float dt,
            const component::InputRepeatConfig& config
        );

        bool moveMenuUp(float deltaTime, component::InputComponent& input, const component::InputRepeatConfig& repeatConfig);
        bool moveMenuDown(float deltaTime, component::InputComponent& input, const component::InputRepeatConfig& repeatConfig);
        bool menuItemConfirmed(component::InputComponent& input);
        bool isMouseInsideButton(const glm::vec2& mousePos, const component::Transform& buttonTransform);

        void playMoveSound();
        void playConfirmSound();

        void updateFocusedVisuals(const component::UIFocus& focus);
        void updateFocusedVisualsNineSlice(const component::UIFocus& focus);
        void updateFocusedVisualsContainer(const component::UIFocus& focus);
        void updateFocusedVisualsContainer(const bool focus, entt::entity entity);

        void handleActivation
        (
            component::InputComponent& input,
            component::UIFocus& focus
        );
        int findButtonIndex(entt::entity button, const std::vector<entt::entity>& children);
        entt::entity findClickedButton(const glm::vec2& mousePos, const std::vector<entt::entity>& children);
        void activateButton(entt::entity button);

    };

}
