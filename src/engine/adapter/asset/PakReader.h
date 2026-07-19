#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <zlib.h>

#include "engine/core/asset/IAssetReader.h"
#include "engine/adapter/asset/EncryptionUtil.h"

namespace niketica::asset
{
    struct FileEntry {
        uint32_t offset;
        uint32_t compressed_size;
        uint32_t original_size;
    };

    class PakReader : public IAssetReader {
    public:
        explicit PakReader(const char* passphrase) : passphrase(passphrase) {}
        ~PakReader() = default;

        bool load(const std::string& pak_file);
        bool hasFile(const std::string& filename) const;
        std::vector<uint8_t> getFile(const std::string& filename);

    private:
        const char* passphrase;

        bool loaded = false;

        std::ifstream pak_stream;
        std::unordered_map<std::string, FileEntry> entries;

        std::vector<uint8_t> decompressData(const std::vector<uint8_t>& compressed, size_t original_size);

        uint32_t read_u32_le(std::istream& in);
    };
}
