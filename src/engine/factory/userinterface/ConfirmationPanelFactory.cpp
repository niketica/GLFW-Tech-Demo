#include "engine/factory/userinterface/ConfirmationPanelFactory.h"

namespace niketica::factory::ui
{
    ConfirmationPanelFactory::ConfirmationPanel ConfirmationPanelFactory::createConfirmationPanel(const std::string& text)
    {
        std::vector<std::string> lines = niketica::util::string::splitLines(text);
        return createConfirmationPanel(lines);
    }

    ConfirmationPanelFactory::ConfirmationPanel ConfirmationPanelFactory::createConfirmationPanel(const std::vector<std::string>& lines)
    {
        float fontSize = 20.0f;
        float spacing = 20.0f;
        float marginButtonsValue = 20.0f;
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

        auto containerBox = createContainerRect("204523", "09140A", glm::vec2{width, height}, 16.0f);
        auto buttonConfirmC = createButton("Confirm", BUTTON_CONFIRM_SIZE, BUTTON_PADDING);
        auto buttonCancel = createButton("Cancel", BUTTON_CONFIRM_SIZE, BUTTON_PADDING);

        auto buttonContainer = createContainer({ BUTTON_CONFIRM_SIZE.x * 2.0f, BUTTON_CONFIRM_SIZE.y }, spacing, niketica::component::UILayoutType::HORIZONTAL);
        addChildToContainer(buttonContainer, buttonConfirmC);
        addChildToContainer(buttonContainer, buttonCancel);
        niketica::component::UIMargin marginButtons;
        marginButtons.top = marginButtonsValue;
        registry->emplace<niketica::component::UIMargin>(buttonContainer, marginButtons);
        std::vector<entt::entity> textLabels;
        for (const auto& line : lines)
        {
            const auto label = createTextLabel(line.c_str(), fontSize);
            addChildToContainer(containerBox, label);
            textLabels.emplace_back(label);
        }
        
        addChildToContainer(containerBox, buttonContainer);

        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(containerBox);
        anchor.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        anchor.vertical = niketica::component::AlignmentVertical::CENTER;
        anchor.offset.x = width * -0.5f;
        anchor.offset.y = height * -0.5f;
        registry->emplace<niketica::component::UIContainerLayoutDirty>(containerBox);

        return
        {
            containerBox,
            textLabels,
            buttonConfirmC,
            buttonCancel
        };
    }

}
