#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <miniaudio.h>

#include "engine/asset/File.h"
#include "engine/asset/AssetManager.h"

namespace niketica::sound
{
    class SoundBackendMiniaudio
    {
    public:
        SoundBackendMiniaudio() = default;
        ~SoundBackendMiniaudio() = default;

        void init();
        void update();
        void end();
        void playSoundSystemPath(const std::string& filePath);
        void playSoundAssetFile(const char* path, bool loop = false);

    private:
        ma_engine soundEngine;

        std::vector<std::unique_ptr<ma_sound>> activeSounds;
        std::vector<std::unique_ptr<ma_decoder>> activeDecoders;

    };

}
