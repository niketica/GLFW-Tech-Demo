#pragma once

namespace niketica::sound
{

    class ISoundContext
    {
    public:
        virtual ~ISoundContext() = default;
        virtual void init() = 0;
        virtual void playSoundAssetFile(const char* path, bool loop = false) = 0;
    };
}
