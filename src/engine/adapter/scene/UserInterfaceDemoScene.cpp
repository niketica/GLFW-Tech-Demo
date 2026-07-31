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

        createMainMenuPanel();
        createTestPanel2();
    }

    void UserInterfaceDemoScene::input()
    {
        systemContext->input();

        auto inputView = registry->view<niketica::component::InputComponent>();
        auto& input = inputView.get<niketica::component::InputComponent>(inputView.front());

        if (input.actions[niketica::component::Action::MINUS].pressed)
        {
            auto sceneSwitch = niketica::component::SceneSwitchInstruction{ niketica::component::SceneType::TEST };
            registry->emplace<niketica::component::SceneSwitchInstruction>(registry->create(), sceneSwitch);
        }

    }

    void UserInterfaceDemoScene::update(float deltaTime)
    {
        systemContext->update(deltaTime);

        auto viewButtonActivated = registry->view<niketica::component::ButtonActivated, Button>();
        for (auto entity : viewButtonActivated)
        {
            registry->remove<niketica::component::ButtonActivated>(entity);
            auto buttonType = registry->get<Button>(entity).type;

            switch(buttonType)
            {
            case ButtonType::START:
                std::cout << "Start button activated" << std::endl;
                break;
            case ButtonType::OPTIONS:
            {
                std::cout << "Options button activated" << std::endl;                
                const auto& parentTransform = registry->get<niketica::component::ParentTransform>(entity);
                destroyUIElement(parentTransform.parent);
                createOptionsMenuPanel();                
            }
                break;
            case ButtonType::QUIT:
            {
                std::cout << "Quit button activated" << std::endl;
                auto viewEngineConfig = registry->view<niketica::component::EngineConfig>();
                auto& engineConfig = viewEngineConfig.get<niketica::component::EngineConfig>(viewEngineConfig.front());
                engineConfig.running = false;                
            }
                break;
            case ButtonType::OPTIONS_BACK:
            {
                std::cout << "Options back activated" << std::endl;                
                const auto& parentTransform = registry->get<niketica::component::ParentTransform>(entity);
                destroyUIElement(parentTransform.parent);
                createMainMenuPanel();                
            }
                break;
            case ButtonType::OPTIONS_RESOLUTION_800x600:
            {
                auto viewWindow = registry->view<niketica::component::Window>();
                auto& window = viewWindow.get<niketica::component::Window>(viewWindow.front());
                engineServices->getRenderContext()->setWindowMode(window, niketica::component::WindowMode::WINDOWED, 800, 600, 0);
                
                auto viewRootContainers = registry->view<niketica::component::UILayout>(entt::exclude<component::ParentTransform>);
                for (auto rootConainer : viewRootContainers)
                {
                    registry->emplace<niketica::component::UILayoutDirty>(rootConainer);
                }
            }
                break;
            case ButtonType::OPTIONS_RESOLUTION_1920x1080:
            {
                auto viewWindow = registry->view<niketica::component::Window>();
                auto& window = viewWindow.get<niketica::component::Window>(viewWindow.front());
                engineServices->getRenderContext()->setWindowMode(window, niketica::component::WindowMode::WINDOWED, 1920, 1080, 0);
                
                auto viewRootContainers = registry->view<niketica::component::UILayout>(entt::exclude<component::ParentTransform>);
                for (auto rootConainer : viewRootContainers)
                {
                    registry->emplace<niketica::component::UILayoutDirty>(rootConainer);
                }
            }
                break;
            }
        }
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

    void UserInterfaceDemoScene::createMainMenuPanel()
    {
        float panelWidth = 400.0f;
        float panelHeight = 600.0f;

        auto buttonStart = createButton("Start");
        registry->emplace<Button>(buttonStart, Button{ ButtonType::START });
        auto buttonOptions = createButton("Options");
        registry->emplace<Button>(buttonOptions, Button{ ButtonType::OPTIONS });
        auto buttonQuit = createButton("Quit");
        registry->emplace<Button>(buttonQuit, Button{ ButtonType::QUIT });

        niketica::builder::UIPanelBuilder panelBuilder = { registry, engineServices };
        auto panel = panelBuilder
            .withPosition({600.0f,100.0f})
            .withSize({panelWidth,panelHeight})
            .withPadding(20.0f)
            .withFontColor({ 1.0f, 1.0f, 0.0f, 1.0f })
            .withFontSize(20.0f)
            .withAlignmentHorizontal(niketica::component::AlignmentHorizontal::CENTER)
            .withAlignmentVertical(niketica::component::AlignmentVertical::CENTER)
            .withLayoutType(niketica::component::UILayoutType::VERTICAL)
            .addButton(buttonStart)
            .addButton(buttonOptions)
            .addButton(buttonQuit)
            .build();

        registry->emplace<niketica::component::UIActive>(panel);
        registry->emplace<niketica::component::UIFocus>(panel);
    }

    void UserInterfaceDemoScene::createOptionsMenuPanel()
    {
        float panelWidth = 400.0f;
        float panelHeight = 600.0f;

        auto buttonResolution800x600 = createButton("Set resolution 800x600");
        registry->emplace<Button>(buttonResolution800x600, Button{ ButtonType::OPTIONS_RESOLUTION_800x600 });
        auto buttonResolution1920x1080 = createButton("Set resolution 1920x1080");
        registry->emplace<Button>(buttonResolution1920x1080, Button{ ButtonType::OPTIONS_RESOLUTION_1920x1080 });
        auto buttonBack = createButton("Back");
        registry->emplace<Button>(buttonBack, Button{ ButtonType::OPTIONS_BACK });

        niketica::builder::UIPanelBuilder panelBuilder = { registry, engineServices };
        auto panel = panelBuilder
            .withPosition({600.0f,100.0f})
            .withSize({panelWidth,panelHeight})
            .withPadding(20.0f)
            .withFontColor({ 1.0f, 1.0f, 0.0f, 1.0f })
            .withFontSize(20.0f)
            .withAlignmentHorizontal(niketica::component::AlignmentHorizontal::CENTER)
            .withAlignmentVertical(niketica::component::AlignmentVertical::CENTER)
            .withLayoutType(niketica::component::UILayoutType::VERTICAL)
            .addButton(buttonResolution800x600)
            .addButton(buttonResolution1920x1080)
            .addButton(buttonBack)
            .build();

        registry->emplace<niketica::component::UIActive>(panel);
        registry->emplace<niketica::component::UIFocus>(panel);
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
            .addTextLabel(createTextLabel("A"))
            .addTextLabel(createTextLabel("B"))
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

    void UserInterfaceDemoScene::destroyUIElement(entt::entity entity)
    {
        if (registry->any_of<niketica::component::UIChildren>(entity))
        {
            const auto& children = registry->get<niketica::component::UIChildren>(entity);
            for (auto child : children.children)
            {
                destroyUIElement(child);
            }
        }
        registry->destroy(entity);
    }

}
