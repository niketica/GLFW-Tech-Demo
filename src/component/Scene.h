#pragma once

namespace niketica::component
{
    
    enum class SceneType
    {
        TEST,
        MAIN_MENU,
        SNAKE,
        TETRIS,
        UI_SAMPLES
    };

    struct SceneSwitchInstruction
    {
        SceneType nextScene;
    };

}
