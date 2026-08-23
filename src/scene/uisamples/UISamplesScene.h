#pragma once

#include <string>
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
#include "engine/util/EngineUtils.h"
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
            CREATE_CONFIRM_BOX,
            CREATE_CHECKBOX_BOX,
            CREATE_RADIO_BOX,
            CREATE_DROPDOWN_BOX,
            CREATE_DRAGABLE_BOX,
            INFO_BOX_OK,
            CONFIRM_BOX_CONFIRM,
            CONFIRM_BOX_CANCEL
        };

        struct ButtonScene
        {
            ButtonType type = ButtonType::UNDEFINED;
        };

        struct TempPanel
        {
            entt::entity rootContainer;
            std::vector<entt::entity> textLabels;
        };

        struct ConfirmationPanel
        {
            entt::entity rootContainer;
            std::vector<entt::entity> textLabels;
            entt::entity buttonConfirm;
            entt::entity buttonCancel;
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
        const glm::vec2 BUTTON_CONFIRM_SIZE = { 100.0f, 40.0f};
        const float BUTTON_PADDING = 20.0f;
        const float BUTTON_MAIN_OFFSET_X = 100.0f;
        const float BUTTON_MAIN_OFFSET_Y_START = -200.0f;
        const float BUTTON_MAIN_OFFSET_Y_SPACING = (BUTTON_MAIN_SIZE.y + 20.0f) * -1.0f;
        float buttonMainOffsetY = BUTTON_MAIN_OFFSET_Y_START;

        entt::entity buttonInfo;
        entt::entity buttonTemp;
        entt::entity buttonConfirm;
        entt::entity buttonCheckbox;
        entt::entity buttonRadio;
        entt::entity buttonDropdown;
        entt::entity buttonDragable;

        entt::entity textTTL;

        void init();

        // Main demo buttons
        void createMainButtons();
        entt::entity createMainButton(const char* text, ButtonType type);

        void createInfoBox();
        TempPanel createTempPanel(const float ttl, const std::string& text);
        TempPanel createTempPanel(const float ttl, const std::vector<std::string>& lines);
        ConfirmationPanel createConfirmationPanel(const std::string& text);
        ConfirmationPanel createConfirmationPanel(const std::vector<std::string>& lines);
        void createCheckboxBox();
        void createRadioBox();
        void createDropdownBox();
        void createDragableBox();

        entt::entity createContainer(const glm::vec2& size, const float spacing, const niketica::component::UILayoutType layout);
        void makeContainer(entt::entity, const glm::vec2& size, const float spacing, const niketica::component::UILayoutType layout);

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
