#include "scene/uisamples/UISamplesScene.h"

namespace niketica::scene
{
    UISamplesScene::UISamplesScene
    (
        entt::registry* registry,
        niketica::engine::EngineServices* engineServices
    ) : registry(registry), engineServices(engineServices)
    {
        init();
    }

    void UISamplesScene::init()
    {
        systemContext = std::make_unique<niketica::systems::SystemContext>(registry, engineServices);
        systemContext->init();

        auto windowView = registry->view<niketica::component::Window>();
        const auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        auto textColor = niketica::util::color::colorFromHexRGB("09140A");
        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        textLabelBuilder
            .withText("User Interface Samples")
            .withFontSize(48.0f)
            .withFontType(niketica::component::FontType::OPEN_SANS_REGULAR)
            .withColor(textColor)
            .withPosition(glm::vec2{ 100.0f, (float)windowComponent.height - 100.0f })
            .withAnchor({ niketica::component::AlignmentHorizontal::LEFT, niketica::component::AlignmentVertical::TOP, { 100.0f, -100.0f } })
            .build();

        createMainButtons();
        setFocusOnMainButtons();

        registry->emplace<niketica::component::UIGlobalLayoutDirty>(registry->create());
    }

    void UISamplesScene::input()
    {
        systemContext->input();

        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::ESCAPE].pressed)
        {
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::MAIN_MENU };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }

    }

    void UISamplesScene::update(float dt)
    {
        systemContext->update(dt);

        auto viewButtonActivated = registry->view<niketica::component::ButtonActivated>();
        for (auto entity : viewButtonActivated)
        {
            registry->remove<niketica::component::ButtonActivated>(entity);

            if (registry->all_of<ButtonScene>(entity))
            {
                const auto& buttonType = registry->get<ButtonScene>(entity).type;

                switch (buttonType)
                {
                case ButtonType::CREATE_INFO_BOX:
                {
                    createInfoBox();
                    removeFocusOnMainButtons();
                }
                    break;
                case ButtonType::CREATE_TEMP_BOX:
                {
                    createTempBox();
                    removeFocusOnMainButtons();
                }
                    break;
                case ButtonType::CREATE_CONFIRM_BOX:
                {
                    auto panel = createConfirmationPanel("This is a confirmation box.\nClick Confirm or Cancel to close it.");
                    registry->emplace<ButtonScene>(panel.buttonConfirm, ButtonScene{ ButtonType::CONFIRM_BOX_CONFIRM });
                    registry->emplace<ButtonScene>(panel.buttonCancel, ButtonScene{ ButtonType::CONFIRM_BOX_CANCEL });

                    niketica::util::ui::clearFocusables(registry);
                    niketica::util::ui::addFocusable(registry, panel.buttonConfirm);
                    niketica::util::ui::addFocusable(registry, panel.buttonCancel);

                    removeFocusOnMainButtons();
                }
                    break;
                case ButtonType::CREATE_CHECKBOX_BOX:
                {
                    createCheckboxBox();
                    removeFocusOnMainButtons();
                }
                    break;
                case ButtonType::CREATE_RADIO_BOX:
                {
                    createRadioBox();
                    removeFocusOnMainButtons();
                }
                    break;
                case ButtonType::CREATE_DROPDOWN_BOX:
                {
                    createDropdownBox();
                    removeFocusOnMainButtons();
                }
                    break;
                case ButtonType::CREATE_DRAGABLE_BOX:
                {
                    createDragableBox();
                    removeFocusOnMainButtons();
                }
                    break;
                case ButtonType::INFO_BOX_OK:
                case ButtonType::CONFIRM_BOX_CONFIRM:
                case ButtonType::CONFIRM_BOX_CANCEL:
                {
                    auto root = getContainerRoot(entity);
                    destroyContainer(root);
                    setFocusOnMainButtons();
                }
                    break;
                }
            }
        }

        auto viewTTL = registry->view<niketica::component::TimeToLive>();
        for (auto entity : viewTTL)
        {
            auto& ttl = registry->get<niketica::component::TimeToLive>(entity);
            ttl.currentLiveTime += dt;
            if (ttl.currentLiveTime >= ttl.timeToLive)
            {
                destroyContainer(entity);
                setFocusOnMainButtons();
            }
        }

        if (textTTL != entt::null && registry->all_of<niketica::component::Text>(textTTL))
        {
            // For now we can just assume this particular entity always has a parent.
            const auto& parent = registry->get<niketica::component::ParentTransform>(textTTL).parent;
            const auto& ttl = registry->get<niketica::component::TimeToLive>(parent);
            auto& text = registry->get<niketica::component::Text>(textTTL);

            auto remaining = ttl.timeToLive - ttl.currentLiveTime;
            std::string strRemaining = niketica::util::string::parseFloat(remaining, 2);
            text.value = "Remaining time to live: " + strRemaining;

            niketica::util::ui::updateTextSize(registry, textTTL);
            registry->emplace_or_replace<niketica::component::UIContainerLayoutDirty>(parent);
        }
    }
    
    void UISamplesScene::render()
    {
        systemContext->render();
    }
    
    void UISamplesScene::reset()
    {        
        systemContext.release();
        init();
    }
    
    void UISamplesScene::createMainButtons()
    {
        buttonMainOffsetY = BUTTON_MAIN_OFFSET_Y_START;
        buttonInfo = createMainButton("Create Info Box", ButtonType::CREATE_INFO_BOX);
        buttonMainOffsetY += BUTTON_MAIN_OFFSET_Y_SPACING;
        buttonTemp = createMainButton("Create Temporary Box", ButtonType::CREATE_TEMP_BOX);
        buttonMainOffsetY += BUTTON_MAIN_OFFSET_Y_SPACING;
        buttonConfirm = createMainButton("Create Confirm Box", ButtonType::CREATE_CONFIRM_BOX);
        buttonMainOffsetY += BUTTON_MAIN_OFFSET_Y_SPACING;
        buttonCheckbox = createMainButton("Create Checkbox Box", ButtonType::CREATE_CHECKBOX_BOX);
        buttonMainOffsetY += BUTTON_MAIN_OFFSET_Y_SPACING;
        buttonRadio = createMainButton("Create Radio Box", ButtonType::CREATE_RADIO_BOX);
        buttonMainOffsetY += BUTTON_MAIN_OFFSET_Y_SPACING;
        buttonDropdown = createMainButton("Create Dropdown Box", ButtonType::CREATE_DROPDOWN_BOX);
        buttonMainOffsetY += BUTTON_MAIN_OFFSET_Y_SPACING;
        buttonDragable = createMainButton("Create Dragable Box", ButtonType::CREATE_DRAGABLE_BOX);
    }

    void UISamplesScene::setFocusOnMainButtons()
    {
        niketica::util::ui::setFocusables
        (
            registry,
            {
                buttonInfo,
                buttonTemp,
                buttonConfirm,
                buttonCheckbox,
                buttonRadio,
                buttonDropdown,
                buttonDragable
            }
        );
    }

    void UISamplesScene::removeFocusOnMainButtons()
    {
        niketica::util::ui::updateFocusedVisualsContainer(registry, false, buttonInfo);
        niketica::util::ui::updateFocusedVisualsContainer(registry, false, buttonTemp);
        niketica::util::ui::updateFocusedVisualsContainer(registry, false, buttonConfirm);
        niketica::util::ui::updateFocusedVisualsContainer(registry, false, buttonCheckbox);
        niketica::util::ui::updateFocusedVisualsContainer(registry, false, buttonRadio);
        niketica::util::ui::updateFocusedVisualsContainer(registry, false, buttonDropdown);
        niketica::util::ui::updateFocusedVisualsContainer(registry, false, buttonDragable);
    }

    entt::entity UISamplesScene::createMainButton(const char* text, ButtonType type)
    {
        auto button = createButton(text, BUTTON_MAIN_SIZE, BUTTON_PADDING);
        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(button);
        anchor.horizontal = niketica::component::AlignmentHorizontal::LEFT;
        anchor.vertical = niketica::component::AlignmentVertical::TOP;
        anchor.offset.x = BUTTON_MAIN_OFFSET_X;
        anchor.offset.y = buttonMainOffsetY;

        registry->emplace<ButtonScene>(button, ButtonScene{ type });

        return button;
    }
    
    void UISamplesScene::createInfoBox()
    {
        float width = 300.0f;
        float height = 160.0f;
        auto containerBox = createContainerRect("204523", "09140A", glm::vec2{width, height}, 16.0f);
        auto textInfo1 = createTextLabel("This is an info box.", 20.0f);
        auto textInfo2 = createTextLabel("Click OK to close it.", 20.0f);
        auto okButton = createButton("OK", BUTTON_OK_SIZE, BUTTON_PADDING);
        addChildToContainer(containerBox, textInfo1);
        addChildToContainer(containerBox, textInfo2);
        addChildToContainer(containerBox, okButton);

        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(containerBox);
        anchor.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        anchor.vertical = niketica::component::AlignmentVertical::CENTER;
        anchor.offset.x = width * -0.5f;
        anchor.offset.y = height * -0.5f;
        registry->emplace<niketica::component::UIContainerLayoutDirty>(containerBox);

        niketica::util::ui::clearFocusables(registry);
        niketica::util::ui::addFocusable(registry, okButton);

        registry->emplace<ButtonScene>(okButton, ButtonScene{ ButtonType::INFO_BOX_OK });
    }

    void UISamplesScene::createTempBox()
    {
        float width = 400.0f;
        float height = 120.0f;
        auto containerBox = createContainerRect("204523", "09140A", glm::vec2{width, height}, 16.0f);
        auto textInfo1 = createTextLabel("This is a temporary box and", 20.0f);
        auto textInfo2 = createTextLabel("will close automatically.", 20.0f);
        textTTL = createTextLabel("Time to live: ", 20.0f);
        addChildToContainer(containerBox, textInfo1);
        addChildToContainer(containerBox, textInfo2);
        addChildToContainer(containerBox, textTTL);

        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(containerBox);
        anchor.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        anchor.vertical = niketica::component::AlignmentVertical::CENTER;
        anchor.offset.x = width * -0.5f;
        anchor.offset.y = height * -0.5f;
        registry->emplace<niketica::component::UIContainerLayoutDirty>(containerBox);

        niketica::util::ui::clearFocusables(registry);
        registry->emplace<niketica::component::TimeToLive>(containerBox, niketica::component::TimeToLive{ 3.0f });
    }
    
    UISamplesScene::ConfirmationPanel UISamplesScene::createConfirmationPanel(const std::string& text)
    {
        std::vector<std::string> lines = niketica::util::string::splitLines(text);
        return createConfirmationPanel(lines);
    }

    UISamplesScene::ConfirmationPanel UISamplesScene::createConfirmationPanel(const std::vector<std::string>& lines)
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

        for (const auto& line : lines)
        {
            const auto label = createTextLabel(line.c_str(), fontSize);
            addChildToContainer(containerBox, label);
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
            buttonConfirmC,
            buttonCancel
        };
    }

    void UISamplesScene::createCheckboxBox()
    {
        // TODO implementation
        std::cout << "Not yet implemented!" << std::endl;
    }

    void UISamplesScene::createRadioBox()
    {
        // TODO implementation
        std::cout << "Not yet implemented!" << std::endl;
    }
    
    void UISamplesScene::createDropdownBox()
    {
        // TODO implementation
        std::cout << "Not yet implemented!" << std::endl;
    }
    
    void UISamplesScene::createDragableBox()
    {
        // TODO implementation
        std::cout << "Not yet implemented!" << std::endl;
    }

    entt::entity UISamplesScene::createButton(const char* text, const glm::vec2& size, const float padding)
    {
        auto textLabel = createTextLabel(text, 20.0f);
        auto button = createContainerRect("152E18", "09140A", size);
        setPadding(button, padding);
        addChildToContainer(button, textLabel);
        return button;
    }

    entt::entity UISamplesScene::createTextLabel(const char* text, float fontSize)
    {
        auto textNormalColor = niketica::util::color::colorFromHexRGB("000000");
        auto textHighlightColor = niketica::util::color::colorFromHexRGB("FFFFFF");

        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        auto entity = textLabelBuilder
            .withText(text)
            .withFontSize(fontSize)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .withColor(textNormalColor)
            .withPosition(glm::vec2{ 0.0f, 0.0f })
            .build();

        registry->emplace<niketica::component::UINormalColor>(entity, niketica::component::UINormalColor{ textNormalColor });
        registry->emplace<niketica::component::UIHighlightColor>(entity, niketica::component::UIHighlightColor{ textHighlightColor });
            
        niketica::component::UIAlignment aligment;
        aligment.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        aligment.vertical = niketica::component::AlignmentVertical::CENTER;
        registry->emplace<niketica::component::UIAlignment>(entity, aligment);

        return entity;
    }

    entt::entity UISamplesScene::createContainerRect(const char* fillColor, const char* borderColor, const glm::vec2 size, float spacing)
    {
        auto fillColorVec4 = niketica::util::color::colorFromHexRGB(fillColor);
        auto borderColorVec4 = niketica::util::color::colorFromHexRGB(borderColor);
        niketica::builder::UIRectangleBuilder rectBuilder = { registry, engineServices };
        auto entity = rectBuilder
            .withSize(size)
            .withFillColor(fillColorVec4)
            .withBorderColor(borderColorVec4)
            .withBorderThickness(4.0f)
            .build();

        makeContainer(entity, size, spacing, niketica::component::UILayoutType::VERTICAL);

        return entity;
    }

    void UISamplesScene::addChildToContainer(entt::entity container, entt::entity child)
    {
        if (!registry->all_of<niketica::component::Transform>(child))
        {
            registry->emplace<niketica::component::Transform>(child);
        }
        if (!registry->all_of<niketica::component::LocalTransform>(child))
        {
            registry->emplace<niketica::component::LocalTransform>(child);
        }

        niketica::component::ParentTransform parentTransform = { container };
        registry->emplace<niketica::component::ParentTransform>(child, parentTransform);

        if (registry->any_of<niketica::component::UIChildren>(container))
        {
            auto& children = registry->get<niketica::component::UIChildren>(container).children;
            children.emplace_back(child);
        }
        else
        {
            niketica::component::UIChildren containerChildren;
            containerChildren.children.emplace_back(child);
            registry->emplace<niketica::component::UIChildren>(container, containerChildren);
        }
    }

    void UISamplesScene::setPadding(entt::entity entity, const float value)
    {
        auto& padding = registry->get_or_emplace<niketica::component::UIContentPadding>(entity);
        padding.top = value;
        padding.bottom = value;
        padding.left = value;
        padding.right = value;
    }

    glm::vec2 UISamplesScene::getSizeWithPadding(glm::vec2 size, float padding) const
    {
        return glm::vec2{ size.x + padding, size.y + padding };
    }
    
    entt::entity UISamplesScene::getContainerRoot(entt::entity entity)
    {
        if (registry->all_of<niketica::component::ParentTransform>(entity))
        {
            const auto& parent = registry->get<niketica::component::ParentTransform>(entity).parent;
            return getContainerRoot(parent);
        }
        return entity;
    }
    
    void UISamplesScene::destroyContainer(entt::entity entity)
    {
        if (registry->all_of<niketica::component::UIChildren>(entity))
        {
            const auto& children = registry->get<niketica::component::UIChildren>(entity).children;
            for (auto child : children)
            {
                destroyContainer(child);
            }
        }
        registry->destroy(entity);
    }

    entt::entity UISamplesScene::createContainer(const glm::vec2& size, const float spacing, const niketica::component::UILayoutType layout)
    {
        auto entity = registry->create();
        makeContainer(entity, size, spacing, layout);
        return entity;
    }

    void UISamplesScene::makeContainer(entt::entity entity, const glm::vec2& size, const float spacing, const niketica::component::UILayoutType layout)
    {
        niketica::component::UIAlignment aligment;
        aligment.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        aligment.vertical = niketica::component::AlignmentVertical::CENTER;

        niketica::component::UILayout layoutCmpnt;
        layoutCmpnt.type = layout;
        niketica::component::UIPadding padding;
        niketica::component::UIContentPadding contentPadding;
        niketica::component::UISpacing spacingCmpnt;
        spacingCmpnt.spacing = spacing;
        niketica::component::UISize uiSize;
        uiSize.width = size.x;
        uiSize.height = size.y;

        registry->emplace<niketica::component::UIAlignment>(entity, aligment);
        registry->emplace<niketica::component::UILayout>(entity, layoutCmpnt);
        registry->emplace<niketica::component::UIPadding>(entity, padding);
        registry->emplace<niketica::component::UIContentPadding>(entity, contentPadding);
        registry->emplace<niketica::component::UISpacing>(entity, spacingCmpnt);
        registry->emplace<niketica::component::UISize>(entity, uiSize);
        registry->emplace<niketica::component::UIChildren>(entity);
    }
    
}
