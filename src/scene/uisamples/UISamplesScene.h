#pragma once

#include <vector>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <entt/entt.hpp>

#include "component/Components.h"
#include "engine/core/EngineServices.h"
#include "engine/core/scene/IScene.h"
#include "engine/core/systems/ISystemContext.h"
#include "engine/util/ColorUtil.h"
#include "engine/util/UserInterfaceUtil.h"
#include "systems/SystemContext.h"
#include "engine/builder/userinterface/UITextLabelBuilder.h"
#include "engine/builder/userinterface/UIRectangleBuilder.h"

namespace niketica::scene
{

    class UISamplesScene : public IScene
    {
    public:
        
        enum class ButtonType
        {
            UNDEFINED,
            CREATE_INFO_BOX,
            CREATE_TEMP_BOX,
            INFO_BOX_OK
        };

        struct ButtonScene
        {
            ButtonType type = ButtonType::UNDEFINED;
        };

        UISamplesScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        );
        ~UISamplesScene() = default;

        void input() override;
        void update(float dt) override;
        void render() override;
        void reset() override;
    
    private:
        entt::registry* registry;
        std::unique_ptr<niketica::systems::ISystemContext> systemContext;
        niketica::engine::EngineServices* engineServices;

        const glm::vec2 BUTTON_MAIN_SIZE = { 320.0f, 60.0f };
        const glm::vec2 BUTTON_OK_SIZE = { 60.0f, 40.0f};
        const float BUTTON_PADDING = 20.0f;

        entt::entity buttonCreateInfoBox;
        entt::entity buttonTest;

        void init();
        void createInfoButton();
        void createTempButton();
        void createInfoBox();
        void createTempBox();

        entt::entity createButton(const char* text, const glm::vec2& size, const float padding);
        entt::entity createTextLabel(const char* text, float fontSize);
        entt::entity createContainerRect(const char* fillColor, const char* borderColor, const glm::vec2 size, float spacing = 0.0f);
        void addChildToContainer(entt::entity container, entt::entity child);

        glm::vec2 getSizeWithPadding(glm::vec2 size, float padding) const;
        void setPadding(entt::entity entity, const float value);

        entt::entity getContainerRoot(entt::entity entity);
        void destroyContainer(entt::entity entity);

        void setFocusOnMainButtons();
        void removeFocusOnMainButtons();

    };
}
