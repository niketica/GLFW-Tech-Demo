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

        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        textLabelBuilder
            .withText("User Interface Sample")
            .withFontSize(48.0f)
            .withFontType(niketica::component::FontType::OPEN_SANS_REGULAR)
            .withColor(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f})
            .withPosition(glm::vec2{ 100.0f, (float)windowComponent.height - 100.0f })
            .withAnchor({ niketica::component::AlignmentHorizontal::LEFT, niketica::component::AlignmentVertical::TOP, { 100.0f, -100.0f } })
            .build();
        
        // niketica::builder::UIRectangleBuilder rectBuilder = { registry, engineServices };
        // rectBuilder
        //     .withPosition(glm::vec3{400.0f, 400.0f, 0.0f})
        //     .withSize(glm::vec2{400.0f, 400.0f})
        //     .withFillColor(glm::vec4{0.8f, 0.6f, 0.8f, 1.0f})
        //     .withBorderColor(glm::vec4{0.8f, 0.2f, 0.0f, 1.0f})
        //     .withBorderThickness(10.0f)
        //     .build();
        
        // rectBuilder = { registry, engineServices };
        // rectBuilder
        //     .withPosition(glm::vec3{600.0f, 200.0f, 0.0f})
        //     .withSize(glm::vec2{1200.0f, 300.0f})
        //     .withoutFill()
        //     .withBorderColor(glm::vec4{0.0f, 0.2f, 0.8f, 1.0f})
        //     .withBorderThickness(10.0f)
        //     .build();

        niketica::util::clearFocusables(registry);
        //createInfoBox();
        createInfoButton();
        createTempButton();
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
            std::cout << "INFO::UISamplesScene::update - Button activated!" << std::endl;

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
                case ButtonType::INFO_BOX_OK:
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
    
    void UISamplesScene::createInfoButton()
    {
        buttonCreateInfoBox = createButton("Create Info Box");
        const auto& size = registry->get_or_emplace<niketica::component::UISize>(buttonCreateInfoBox);
        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(buttonCreateInfoBox);
        anchor.horizontal = niketica::component::AlignmentHorizontal::LEFT;
        anchor.vertical = niketica::component::AlignmentVertical::TOP;
        anchor.offset.x = 100.0f;
        anchor.offset.y = -200.0f - size.height;

        registry->emplace<ButtonScene>(buttonCreateInfoBox, ButtonScene{ ButtonType::CREATE_INFO_BOX });
    }

    void UISamplesScene::createTempButton()
    {
        buttonTest = createButton("Create Temporary Box");
        const auto& size = registry->get_or_emplace<niketica::component::UISize>(buttonTest);
        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(buttonTest);
        anchor.horizontal = niketica::component::AlignmentHorizontal::LEFT;
        anchor.vertical = niketica::component::AlignmentVertical::TOP;
        anchor.offset.x = 100.0f;
        anchor.offset.y = -300.0f - size.height;

        registry->emplace<ButtonScene>(buttonTest, ButtonScene{ ButtonType::CREATE_TEMP_BOX });
    }

    void UISamplesScene::createInfoBox()
    {
        float width = 400.0f;
        float height = 300.0f;
        auto containerBox = createContainerRect("204523", "09140A", glm::vec2{width, height}, 16.0f);
        auto textInfo = createTextLabel("This is an info box.", 20.0f);
        auto okButton = createButton("OK");
        addChildToContainer(containerBox, textInfo);
        addChildToContainer(containerBox, okButton);

        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(containerBox);
        anchor.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        anchor.vertical = niketica::component::AlignmentVertical::CENTER;
        anchor.offset.x = width * -0.5f;
        anchor.offset.y = height * -0.5f;
        registry->emplace<niketica::component::UIContainerLayoutDirty>(containerBox);

        niketica::util::clearFocusables(registry);
        niketica::util::addFocusable(registry, okButton);

        registry->emplace<ButtonScene>(okButton, ButtonScene{ ButtonType::INFO_BOX_OK });
    }

    void UISamplesScene::createTempBox()
    {
        float width = 400.0f;
        float height = 300.0f;
        auto containerBox = createContainerRect("204523", "09140A", glm::vec2{width, height}, 16.0f);
        auto textInfo = createTextLabel("This is a temporary box and will close automatically.", 20.0f);
        addChildToContainer(containerBox, textInfo);

        auto& anchor = registry->get_or_emplace<niketica::component::UIAnchor>(containerBox);
        anchor.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        anchor.vertical = niketica::component::AlignmentVertical::CENTER;
        anchor.offset.x = width * -0.5f;
        anchor.offset.y = height * -0.5f;
        registry->emplace<niketica::component::UIContainerLayoutDirty>(containerBox);

        niketica::util::clearFocusables(registry);
        registry->emplace<niketica::component::TimeToLive>(containerBox, niketica::component::TimeToLive{ 3.0f });
    }

    entt::entity UISamplesScene::createButton(const char* text)
    {
        auto textLabel = createTextLabel(text, 20.0f);
        const auto& textSize = registry->get_or_emplace<niketica::component::LocalTransform>(textLabel).size;

        float padding = 20.0f;
        auto buttonSize = getSizeWithPadding(textSize, padding);
        auto button = createContainerRect("152E18", "09140A", buttonSize);
        setPadding(button, padding);
        addChildToContainer(button, textLabel);
        return button;
    }

    entt::entity UISamplesScene::createTextLabel(const char* text, float fontSize)
    {
        auto textNormalColor = niketica::util::colorFromHexRGB("000000");
        auto textHighlightColor = niketica::util::colorFromHexRGB("FFFFFF");

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
        auto fillColorVec4 = niketica::util::colorFromHexRGB(fillColor);
        auto borderColorVec4 = niketica::util::colorFromHexRGB(borderColor);

        niketica::component::UIAlignment aligment;
        aligment.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        aligment.vertical = niketica::component::AlignmentVertical::CENTER;

        niketica::component::UILayout layout;
        layout.type = niketica::component::UILayoutType::VERTICAL;
        niketica::component::UIPadding padding;
        niketica::component::UIContentPadding contentPadding;
        niketica::component::UISpacing spacingCmpnt;
        spacingCmpnt.spacing = spacing;
        niketica::component::UISize uiSize;
        uiSize.width = size.x;
        uiSize.height = size.y;

        niketica::builder::UIRectangleBuilder rectBuilder = { registry, engineServices };
        auto entity = rectBuilder
            .withSize(size)
            .withFillColor(fillColorVec4)
            .withBorderColor(borderColorVec4)
            .withBorderThickness(4.0f)
            .build();

        registry->emplace<niketica::component::UIAlignment>(entity, aligment);
        registry->emplace<niketica::component::UILayout>(entity, layout);
        registry->emplace<niketica::component::UIPadding>(entity, padding);
        registry->emplace<niketica::component::UIContentPadding>(entity, contentPadding);
        registry->emplace<niketica::component::UISpacing>(entity, spacingCmpnt);
        registry->emplace<niketica::component::UISize>(entity, uiSize);

        return entity;
    }

    void UISamplesScene::addChildToContainer(entt::entity container, entt::entity child)
    {
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

    void UISamplesScene::setFocusOnMainButtons()
    {
        niketica::util::setFocusables(registry, {buttonCreateInfoBox, buttonTest});
    }

    void UISamplesScene::removeFocusOnMainButtons()
    {
        niketica::util::updateFocusedVisualsContainer(registry, false, buttonCreateInfoBox);
        niketica::util::updateFocusedVisualsContainer(registry, false, buttonTest);
    }

    
}
