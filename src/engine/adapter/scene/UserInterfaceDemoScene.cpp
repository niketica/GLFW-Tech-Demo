#include "engine/adapter/scene/UserInterfaceDemoScene.h"

namespace niketica::scene
{
    UserInterfaceDemoScene::UserInterfaceDemoScene(
            entt::registry* registry,
            niketica::engine::EngineServices* engineServices
        ) : registry(registry),
            engineServices(engineServices)
    {
        init();
    }

    void UserInterfaceDemoScene::init()
    {
        systemContext = std::make_unique<niketica::systems::SystemContext>(registry, engineServices);
        systemContext->init();
        
        auto texture = engineServices->getRenderContext()->getTextureLoader()->acquire("textures/background/forest_001.dds");
        component::Sprite sprite;

        component::Transform transform;
        transform.position = { 0.0f, 0.0f, 0.0f };
        transform.scale = { 1.0f, 1.0f, 1.0f };
        transform.size = { 1920.0f, 1080.0f, 1.0f };

        component::Color color = { { 1.0f,1.0f,1.0f,1.0f } };

        auto entity = registry->create();
        registry->emplace<component::Sprite>(entity, sprite);
        registry->emplace<component::Transform>(entity, transform);
        registry->emplace<component::Color>(entity, color);
        registry->emplace<component::TextureHandle>(entity, texture);
        registry->emplace<component::RenderSprite>(entity);

        auto windowView = registry->view<niketica::component::Window>();
        const auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());

        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        textLabelBuilder
            .withText("User Interface Demo")
            .withFontSize(48.0f)
            .withFontType(niketica::component::FontType::OPEN_SANS_REGULAR)
            .withColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
            .withPosition(glm::vec2{ 100.0f, (float)windowComponent.height - 100.0f })
            .build();

        createTestPanel();
        createTestPanel2();
    }

    void UserInterfaceDemoScene::input()
    {
        systemContext->input();

        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::W].pressed)
        {
            std::cout << "INFO::Engine::update - W key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/397604__nightflame__menu-fx-01.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::A].pressed)
        {
            std::cout << "INFO::Engine::update - A key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/397599__nightflame__menu-fx-02.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::S].pressed)
        {
            std::cout << "INFO::Engine::update - S key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/422514__nightflame__menu-fx-03-normal.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::D].pressed)
        {
            std::cout << "INFO::Engine::update - D key pressed." << std::endl;
            auto sound = niketica::component::SoundEffect{ "sound/sfx/menu/23910__nightflame__menuui-sounds/422515__nightflame__menu-fx-03-descending.wav" };
            registry->emplace<niketica::component::SoundEffect>(registry->create(), sound);
        }
        if (input.actions[niketica::component::Action::MINUS].pressed)
        {
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::TEST };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }

    }

    void UserInterfaceDemoScene::update(float deltaTime)
    {
        systemContext->update(deltaTime);
    }

    void UserInterfaceDemoScene::render()
    {
        systemContext->render();
    }

    void UserInterfaceDemoScene::reset()
    {
        systemContext.release();
        init();
    }

    void UserInterfaceDemoScene::createTestPanel()
    {
        float panelWidth = 400.0f;
        float panelHeight = 600.0f;

        niketica::builder::UIPanelBuilder panelBuilder = { registry, engineServices };
        panelBuilder
            .withPosition({600.0f,100.0f})
            .withSize({panelWidth,panelHeight})
            .withPadding(20.0f)
            .withFontColor({ 1.0f, 1.0f, 0.0f, 1.0f })
            .withFontSize(20.0f)
            .withAlignmentHorizontal(niketica::component::AlignmentHorizontal::CENTER)
            .withAlignmentVertical(niketica::component::AlignmentVertical::CENTER)
            .withLayoutType(niketica::component::UILayoutType::VERTICAL)
            .addTextLabel(createTextLabel("First line"))
            .addTextLabel(createTextLabel("Second line"))
            .addButton(createButton("Start"))
            .addButton(createButton("Options"))
            .addButton(createButton("Quit"))
            .build();
    }

    void UserInterfaceDemoScene::createTestPanel2()
    {
        auto windowView = registry->view<niketica::component::Window>();
        const auto &windowComponent = windowView.get<niketica::component::Window>(windowView.front());
        
        niketica::builder::UIPanelBuilder panelBuilder2 = { registry, engineServices };
        panelBuilder2
            .withPosition({600.0f,100.0f})
            .withSize({windowComponent.width,200.0f})
            .withPadding(20.0f)
            .withFontColor({ 1.0f, 1.0f, 0.0f, 1.0f })
            .withFontSize(20.0f)
            .withAlignmentHorizontal(niketica::component::AlignmentHorizontal::CENTER)
            .withAlignmentVertical(niketica::component::AlignmentVertical::BOTTOM)
            .withLayoutType(niketica::component::UILayoutType::HORIZONTAL)
            .addTextLabel("A")
            .addTextLabel("B")
            .addButton("", { 74.0f, 74.0f})
            .addButton("", { 74.0f, 74.0f})
            .addButton("", { 74.0f, 74.0f})
            .build();
    }
    
    entt::entity UserInterfaceDemoScene::createTextLabel(const std::string& text)
    {
        niketica::builder::UITextLabelBuilder textLabelBuilder = { registry, engineServices };
        return textLabelBuilder
            .withText(text)
            .withColor({ 1.0f, 1.0f, 0.0f, 1.0f })
            .withFontSize(20.0f)
            .withFontType(niketica::component::FontType::COURIER_PRIME_CODE)
            .build();
    }
    
    entt::entity UserInterfaceDemoScene::createButton(const std::string& text)
    {
        niketica::builder::UIButtonBuilder buttonBuilder = { registry, engineServices };
        return buttonBuilder
            .withSize({ 300.0f, 74.0f})
            .withTextLabel(text)
            .withFontColor({ 1.0f, 1.0f, 0.0f, 1.0f })
            .withFontSize(20.0f)
            .withAlignmentHorizontal(niketica::component::AlignmentHorizontal::CENTER)
            .withAlignmentVertical(niketica::component::AlignmentVertical::CENTER)
            .withLayoutType(niketica::component::UILayoutType::VERTICAL)
            .build();
    }

}
