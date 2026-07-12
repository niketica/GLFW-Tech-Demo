#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <zlib.h>

#include "EncryptionUtil.h"
#include "PathUtil.h"

namespace fs = std::filesystem;

namespace niketica::compressor
{
    class PackWriter
    {
    public:
        PackWriter(const char* passphrase) : passphrase(passphrase) {}

        ~PackWriter() = default;

        void packFolder(const std::string& folder_path, const std::string& output_file)
        {
            struct FileEntry {
                std::string filename;
                uint32_t offset;
                uint32_t compressed_size;
                uint32_t original_size;
            };

            std::vector<FileEntry> entries;
            std::vector<std::vector<uint8_t>> file_data;

            for (auto& path : fs::recursive_directory_iterator(folder_path))
            {
                if (!path.is_regular_file()) continue;

                std::ifstream file(path.path(), std::ios::binary);
                std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(file)), {});
                file.close();

                auto compressed = compressData(buffer);

                EncryptionUtil::xorEncrypt(compressed, EncryptionUtil::deriveKey(passphrase));

                std::string rel_path = fs::relative(path.path(), folder_path).string();
                rel_path = normalizePath(rel_path);

                entries.push_back({ rel_path, 0, static_cast<uint32_t>(compressed.size()), static_cast<uint32_t>(buffer.size()) });
                file_data.push_back(std::move(compressed));
            }

            // Calculate offsets
            uint32_t header_size = sizeof(uint32_t);
            for (const auto& e : entries)
            {
                header_size += sizeof(uint32_t) + e.filename.size() + 3 * sizeof(uint32_t);
            }

            uint32_t current_offset = header_size;
            for (auto& entry : entries)
            {
                entry.offset = current_offset;
                current_offset += entry.compressed_size;
            }

            std::ofstream out(output_file, std::ios::binary);
            uint32_t file_count = entries.size();
            out.write(reinterpret_cast<const char*>(&file_count), sizeof(file_count));

            for (const auto& entry : entries)
            {
                uint32_t name_len = entry.filename.size();
                out.write(reinterpret_cast<const char*>(&name_len), sizeof(name_len));
                out.write(entry.filename.c_str(), name_len);
                out.write(reinterpret_cast<const char*>(&entry.offset), sizeof(entry.offset));
                out.write(reinterpret_cast<const char*>(&entry.compressed_size), sizeof(entry.compressed_size));
                out.write(reinterpret_cast<const char*>(&entry.original_size), sizeof(entry.original_size));
            }

            for (const auto& data : file_data)
            {
                out.write(reinterpret_cast<const char*>(data.data()), data.size());
            }

            out.close();
        }

    private:
        const char* passphrase;

        std::vector<uint8_t> compressData(const std::vector<uint8_t>& input)
        {
            uLongf compressed_size = compressBound(input.size());
            std::vector<uint8_t> compressed(compressed_size);

            if (compress(compressed.data(), &compressed_size, input.data(), input.size()) != Z_OK)
            {
                printf("ERROR::PackWriter::compressData - Compression failed!");
                throw std::runtime_error("Compression failed");
            }

            compressed.resize(compressed_size);
            return compressed;
        }
    };
}