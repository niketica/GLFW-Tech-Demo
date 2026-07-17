#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace niketica::asset
{
    extern const char* COMPRESSION_PASSPHRASE;

    class EncryptionUtil
    {
    public:
        static uint8_t deriveKey(const char* passphrase) {
            uint8_t key = 0;
            for (char c : std::string(passphrase))
            {
                key ^= static_cast<uint8_t>(c);
            }
            return key;
        }

        static void xorEncrypt(std::vector<uint8_t>& data, const uint8_t key) {
            for (auto& byte : data) {
                byte ^= key;
            }
        }

        static void xorDecrypt(std::vector<uint8_t>& data, const uint8_t key) {
            for (auto& byte : data) {
                byte ^= key;
            }
        }

    };

}