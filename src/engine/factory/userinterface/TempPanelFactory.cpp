#include "engine/factory/userinterface/TempPanelFactory.h"

namespace niketica::factory::ui
{
    TempPanelFactory::TempPanel TempPanelFactory::createTempPanel(const float ttl, const std::string& text)
    {
        std::vector<std::string> lines = niketica::util::string::splitLines(text);
        return createTempPanel(ttl, lines);
    }

    TempPanelFactory::TempPanel TempPanelFactory::createTempPanel(const float ttl, const std::vector<std::string>& lines)
    {
        float fontSize = 20.0f;
        float spacing = 16.0f;
        float baseHeight = 40.0f;
        float baseWidth = 40.0f;
        float height = baseHeight + ((fontSize + spacing) * (float)lines.size());
        float width = baseWidth;
        for (const auto& line : lines)
        {
            auto lineWidth = baseWidth + (float)((float)line.length() * (fontSize * 0.6));
            if (lineWidth > width)
            {
                width = lineWidth;
            }
        }

        auto containerPanel = createContainerRect("204523", "09140A", glm::vec2{width, height}, spacing);
        std::vector<entt::entity> textLabels;
        for (const auto& line : lines)
        {
            const auto label = createTextLabel(line.c_str(), fontSize);
            addChildToContainer(containerPanel, label);
            textLabels.emplace_back(label);
        }

        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(containerPanel);
        anchor.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        anchor.vertical = niketica::component::AlignmentVertical::CENTER;
        anchor.offset.x = width * -0.5f;
        anchor.offset.y = height * -0.5f;
        registry->emplace<niketica::component::UIContainerLayoutDirty>(containerPanel);

        niketica::util::ui::clearFocusables(registry);
        registry->emplace<niketica::component::TimeToLive>(containerPanel, niketica::component::TimeToLive{ ttl });

        return
        {
            containerPanel,
            textLabels
        };
    }
    
}
