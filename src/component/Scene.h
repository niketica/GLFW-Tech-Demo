#pragma once

namespace niketica::component
{
    
    enum class SceneType
    {
        TEST,
        MAIN_MENU,
        SNAKE
    };

    struct SceneSwitchInstruction
    {
        SceneType nextScene;
    };

}
