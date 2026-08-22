#pragma once

namespace niketica::component
{
    struct Persistent{};

    struct EngineConfig
    {
        bool running = false;
    };

    struct TimeToLive
    {
        float timeToLive = 3.0f;
        float currentLiveTime = 0.0f;
    };

}
