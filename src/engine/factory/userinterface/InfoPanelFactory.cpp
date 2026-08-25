#include "engine/factory/userinterface/InfoPanelFactory.h"

namespace niketica::factory::ui
{
    InfoPanelFactory::InfoPanel InfoPanelFactory::createInfoPanel(const std::string& text)
    {
        std::vector<std::string> lines = niketica::util::string::splitLines(text);
        return createInfoPanel(lines);
    }
    
    InfoPanelFactory::InfoPanel InfoPanelFactory::createInfoPanel(const std::vector<std::string>& lines)
    {
        float fontSize = 20.0f;
        float spacing = 20.0f;
        float baseHeight = 100.0f;
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

        auto containerPanel = createContainerRect("204523", "09140A", glm::vec2{width, height}, 16.0f);
        std::vector<entt::entity> textLabels;
        for (const auto& line : lines)
        {
            const auto label = createTextLabel(line.c_str(), fontSize);
            addChildToContainer(containerPanel, label);
            textLabels.emplace_back(label);
        }

        auto okButton = createButton("OK", BUTTON_OK_SIZE, BUTTON_PADDING);
        addChildToContainer(containerPanel, okButton);

        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(containerPanel);
        anchor.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        anchor.vertical = niketica::component::AlignmentVertical::CENTER;
        anchor.offset.x = width * -0.5f;
        anchor.offset.y = height * -0.5f;
        registry->emplace<niketica::component::UIContainerLayoutDirty>(containerPanel);

        niketica::util::ui::clearFocusables(registry);
        niketica::util::ui::addFocusable(registry, okButton);

        return
        {
            containerPanel,
            textLabels,
            okButton
        };
    }
    
}
