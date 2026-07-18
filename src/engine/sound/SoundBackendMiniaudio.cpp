#include "engine/sound/SoundBackendMiniaudio.h"

#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace niketica::sound
{
    void SoundBackendMiniaudio::init()
    {
        auto result = ma_engine_init(NULL, &soundEngine);
        if (result != MA_SUCCESS) {
            std::cerr << "ERROR::SoundBackendMiniaudio::init - Failed to initialize audio engine: " << result << std::endl;
        }
    }

    void SoundBackendMiniaudio::update()
    {
        for (size_t i = 0; i < activeSounds.size(); )
        {
            if (!ma_sound_is_playing(activeSounds[i].get()))
            {
                ma_sound_uninit(activeSounds[i].get());
                ma_decoder_uninit(activeDecoders[i].get());

                activeSounds.erase(activeSounds.begin() + i);
                activeDecoders.erase(activeDecoders.begin() + i);
            }
            else
            {
                ++i;
            }
        }
    }

    void SoundBackendMiniaudio::end()
    {
        ma_engine_uninit(&soundEngine);
    }

    void SoundBackendMiniaudio::playSoundSystemPath(const std::string& filePath)
    {
        auto result = ma_engine_play_sound(&soundEngine, "C:/ws/git/personal/opengl_tech_demo/assets/sound/sfx/menu/menu_fx_01.wav", NULL);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to initialize sound: " << result << std::endl;
        }
    }

    void SoundBackendMiniaudio::playSoundAssetFile(const char* path, bool loop)
    {
        auto loaded = niketica::asset::AssetManager::Get().Load<niketica::asset::File>(path);
        auto& bytes = loaded->fileData;

        if (bytes.empty()) {
            std::cerr << "ERROR::playSoundAssetFile - File empty\n";
            return;
        }

        // Create decoder from memory
        auto decoder = std::make_unique<ma_decoder>();

        ma_result result = ma_decoder_init_memory(
            bytes.data(),
            bytes.size(),
            NULL,
            decoder.get()
        );

        if (result != MA_SUCCESS) {
            std::cerr << "Failed to init decoder from memory: " << result << std::endl;
            return;
        }

        // Create sound from decoder
        auto sound = std::make_unique<ma_sound>();

        result = ma_sound_init_from_data_source(
            &soundEngine,
            decoder.get(),
            0,
            NULL,
            sound.get()
        );

        if (result != MA_SUCCESS) {
            std::cerr << "Failed to init sound from data source: " << result << std::endl;
            ma_decoder_uninit(decoder.get());
            return;
        }

        if (loop) {
            ma_sound_set_looping(sound.get(), MA_TRUE);
        }

        ma_sound_start(sound.get());

        // Store so they don't get destroyed
        activeDecoders.push_back(std::move(decoder));
        activeSounds.push_back(std::move(sound));
    }

}