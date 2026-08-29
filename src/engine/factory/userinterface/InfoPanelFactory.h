#pragma once

#include <string>
#include <vector>
#include <entt/entt.hpp>

#include "engine/util/EngineUtils.h"
#include "engine/factory/userinterface/BasePanelFactory.h"

namespace niketica::factory::ui
{

    class InfoPanelFactory : public BasePanelFactory
    {
    public:
        struct InfoPanel
        {
            entt::entity rootContainer;
            std::vector<entt::entity> textLabels;
            entt::entity buttonOK;
        };

        InfoPanelFactory(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : BasePanelFactory(registry, engineServices) {};
        ~InfoPanelFactory() = default;

        InfoPanel createInfoPanel(const std::string& text);
        InfoPanel createInfoPanel();

    private:
        const glm::vec2 BUTTON_OK_SIZE = { 60.0f, 40.0f};
        const float BUTTON_PADDING = 20.0f;

        InfoPanel createInfoPanel(const std::vector<std::string>& lines);

    };

}
