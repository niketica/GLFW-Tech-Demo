#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <miniaudio.h>

#include "engine/core/sound/ISoundContext.h"
#include "engine/adapter/asset/File.h"
#include "engine/adapter/asset/AssetManager.h"

namespace niketica::sound
{

    class SoundBackendMiniaudio : public ISoundContext
    {
    public:
        SoundBackendMiniaudio() = default;
        ~SoundBackendMiniaudio() = default;

        void init() override;
        void update();
        void end();
        void playSoundSystemPath(const std::string& filePath);
        void playSoundAssetFile(const char* path, bool loop = false) override;

    private:
        ma_engine soundEngine;

        std::vector<std::unique_ptr<ma_sound>> activeSounds;
        std::vector<std::unique_ptr<ma_decoder>> activeDecoders;

    };

}
