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
        auto containerBox = createContainerRect("204523", "09140A", glm::vec2{400.0f, 300.0f}, 16.0f);
        auto textInfo = createTextLabel("This is an info box.", 20.0f);
        auto okButton = createButton("OK");
        addChildToContainer(containerBox, textInfo);
        addChildToContainer(containerBox, okButton);
    }

    entt::entity UISamplesScene::createButton(const char* text)
    {
        auto okButton = createContainerRect("152E18", "09140A", glm::vec2{40.0f, 30.0f});
        auto textOkButton = createTextLabel("OK", 20.0f);
        addChildToContainer(okButton, textOkButton);
        return okButton;
    }

    entt::entity UISamplesScene::createTextLabel(const char* text, float fontSize)
    {
        auto textColor = niketica::util::colorFromHexRGB("000000");

        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        auto entity = textLabelBuilder
            .withText(text)
            .withFontSize(fontSize)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .withColor(textColor)
            .withPosition(glm::vec2{ 0.0f, 0.0f })
            .build();
            
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

}
