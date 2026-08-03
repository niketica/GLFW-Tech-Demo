#pragma once

#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/builder/userinterface/UITextLabelBuilder.h"
#include "engine/builder/userinterface/UIPanelBuilder.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/adapter/systems/SystemContext.h"

namespace niketica::scene
{

    enum class ButtonType
    {
        START,
        OPTIONS,
        QUIT,
        OPTIONS_WINDOW_800x600,
        OPTIONS_WINDOW_1600x900,
        OPTIONS_WINDOW_1920x1080,
        OPTIONS_RESOLUTION_800x600,
        OPTIONS_RESOLUTION_1600x900,
        OPTIONS_RESOLUTION_1920x1080,
        OPTIONS_WINDOW_MODE_WINDOWED,
        OPTIONS_WINDOW_MODE_BORDERLESS,
        OPTIONS_WINDOW_MODE_FULLSCREEN,
        OPTIONS_BACK
    };

    // 1920x1080
    // 1600x900
    // 1280x720
    // 800x600
    // 3840x2160

    struct Button
    {
        ButtonType type;
    };

    class MainMenuScene : public IScene
    {
    public:
        MainMenuScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~MainMenuScene() = default;

        void input() override;
        void update(float deltaTime) override;
        void render() override;
        void reset() override;
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        void init();
        void createMainMenuPanel();
        void createOptionsMenuPanel();
        void createTestPanel2();
        entt::entity createTextLabel(const std::string& text);
        entt::entity createButton(const std::string& text);

        void destroyUIElement(entt::entity entity);

        void updateWindowSize(const int width, const int height);
        void updateResolution(const int width, const int height);
        void updateWindowMode(niketica::component::WindowMode mode);

    };
}