#pragma once

#include <string>
#include <vector>
#include <entt/entt.hpp>

#include "engine/util/EngineUtils.h"
#include "engine/factory/userinterface/BasePanelFactory.h"

namespace niketica::factory::ui
{

    class ConfirmationPanelFactory : public BasePanelFactory
    {
    public:
        struct ConfirmationPanel
        {
            entt::entity rootContainer;
            std::vector<entt::entity> textLabels;
            entt::entity buttonConfirm;
            entt::entity buttonCancel;
        };

        ConfirmationPanelFactory(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : BasePanelFactory(registry, engineServices) {};
        ~ConfirmationPanelFactory() = default;

        ConfirmationPanel createConfirmationPanel(const std::string& text);

    private:
        const glm::vec2 BUTTON_CONFIRM_SIZE = { 100.0f, 40.0f};
        const float BUTTON_PADDING = 20.0f;
        
        ConfirmationPanel createConfirmationPanel(const std::vector<std::string>& lines);

    };

}
