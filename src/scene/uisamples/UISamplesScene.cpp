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

        createInfoBox();

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

    void UISamplesScene::createInfoBox()
    {
        auto fillColor = niketica::util::colorFromHexRGB("204523");
        auto borderColor = niketica::util::colorFromHexRGB("09140A");

        float boxWidth = 400.0f;
        float boxHeight = 300.0f;

        niketica::builder::UIRectangleBuilder rectBuilder = { registry, engineServices };
        auto containerBox = rectBuilder
            // .withPosition(glm::vec3{100.0f, 20.0f, 0.0f})
            .withSize(glm::vec2{boxWidth, boxHeight})
            .withFillColor(fillColor)
            .withBorderColor(borderColor)
            .withBorderThickness(4.0f)
            .build();
        
        niketica::component::UIAlignment aligment;
        aligment.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        aligment.vertical = niketica::component::AlignmentVertical::CENTER;

        niketica::component::UILayout layout;
        layout.type = niketica::component::UILayoutType::VERTICAL;
        niketica::component::UIPadding padding;
        niketica::component::UIContentPadding contentPadding;
        niketica::component::UISpacing spacing;
        spacing.spacing = 16.0f;
        
        registry->emplace<niketica::component::UIAlignment>(containerBox, aligment);
        registry->emplace<niketica::component::UILayout>(containerBox, layout);
        registry->emplace<niketica::component::UIPadding>(containerBox, padding);
        registry->emplace<niketica::component::UIContentPadding>(containerBox, contentPadding);
        registry->emplace<niketica::component::UISpacing>(containerBox, spacing);

        auto textInfo = createTextLabel("This is an info box.", 20.0f);

        niketica::component::ParentTransform parentTransform = { containerBox };
        registry->emplace<niketica::component::ParentTransform>(textInfo, parentTransform);
        registry->emplace<niketica::component::UIAlignment>(textInfo, aligment);

        auto okButton = createButton("OK");
        niketica::component::ParentTransform parentTransform2 = { containerBox };
        registry->emplace<niketica::component::ParentTransform>(okButton, parentTransform2);

        // ADD CONTAINER CHILDREN
        niketica::component::UIChildren containerChildren;
        containerChildren.children.emplace_back(textInfo);
        containerChildren.children.emplace_back(okButton);
        registry->emplace<niketica::component::UIChildren>(containerBox, containerChildren);
    }

    entt::entity UISamplesScene::createButton(const char* text)
    {
        auto fillColor = niketica::util::colorFromHexRGB("152E18");
        auto borderColor = niketica::util::colorFromHexRGB("09140A");

        niketica::component::UIAlignment aligment;
        aligment.horizontal = niketica::component::AlignmentHorizontal::CENTER;
        aligment.vertical = niketica::component::AlignmentVertical::CENTER;

        niketica::component::UILayout layout;
        layout.type = niketica::component::UILayoutType::VERTICAL;
        niketica::component::UIPadding padding;
        niketica::component::UIContentPadding contentPadding;
        niketica::component::UISpacing spacing;

        niketica::builder::UIRectangleBuilder rectBuilder = { registry, engineServices };
        auto okButton = rectBuilder
            .withSize(glm::vec2{40.0f, 30.0f})
            .withFillColor(fillColor)
            .withBorderColor(borderColor)
            .withBorderThickness(4.0f)
            .build();

        registry->emplace<niketica::component::UIAlignment>(okButton, aligment);
        registry->emplace<niketica::component::UILayout>(okButton, layout);
        registry->emplace<niketica::component::UIPadding>(okButton, padding);
        registry->emplace<niketica::component::UIContentPadding>(okButton, contentPadding);
        registry->emplace<niketica::component::UISpacing>(okButton, spacing);

        auto textOkButton = createTextLabel("OK", 20.0f);

        niketica::component::ParentTransform parentTransform3 = { okButton };
        registry->emplace<niketica::component::ParentTransform>(textOkButton, parentTransform3);
        registry->emplace<niketica::component::UIAlignment>(textOkButton, aligment);

        niketica::component::UIChildren containerChildren;
        containerChildren.children.emplace_back(textOkButton);
        registry->emplace<niketica::component::UIChildren>(okButton, containerChildren);

        return okButton;
    }

    entt::entity UISamplesScene::createTextLabel(const char* text, float fontSize)
    {
        auto textColor = niketica::util::colorFromHexRGB("000000");
        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        textLabelBuilder = { registry, engineServices };
        auto entity = textLabelBuilder
            .withText(text)
            .withFontSize(fontSize)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .withColor(textColor)
            .withPosition(glm::vec2{ 0.0f, 0.0f })
            .build();
        return entity;
    }

}
