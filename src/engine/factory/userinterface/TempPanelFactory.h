#pragma once

#include <string>
#include <vector>
#include <entt/entt.hpp>

#include "engine/util/EngineUtils.h"
#include "engine/factory/userinterface/BasePanelFactory.h"

namespace niketica::factory::ui
{

    class TempPanelFactory : public BasePanelFactory
    {
    public:
        struct TempPanel
        {
            entt::entity rootContainer;
            std::vector<entt::entity> textLabels;
        };

        TempPanelFactory(entt::registry* registry, niketica::engine::EngineServices* engineServices)
            : BasePanelFactory(registry, engineServices) {};
        ~TempPanelFactory() = default;

        TempPanel createTempPanel(const float ttl, const std::string& text);

    private:
        TempPanel createTempPanel(const float ttl, const std::vector<std::string>& lines);

    };

}
