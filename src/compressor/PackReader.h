#pragma once

#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <zlib.h>

namespace niketica::compressor
{
    struct FileEntry {
        uint32_t offset;
        uint32_t compressed_size;
        uint32_t original_size;
    };

    class PakReader {
    public:
        PakReader(const char* passphrase) : passphrase(passphrase) {}

        ~PakReader() = default;

        bool load(const std::string& pak_file)
        {
            printf("PakReader::load - %s\n", pak_file.c_str());
            std::ifstream file(pak_file, std::ios::binary);
            if (!file.is_open())
            {
                printf("ERROR::PakReader::load - Could not open %s!\n", pak_file.c_str());
                return false;
            }

            uint32_t file_count;
            file.read(reinterpret_cast<char*>(&file_count), sizeof(file_count));

            for (uint32_t i = 0; i < file_count; ++i)
            {
                uint32_t name_len;
                file.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
                std::string name(name_len, '\0');
                file.read(&name[0], name_len);

                FileEntry entry;
                file.read(reinterpret_cast<char*>(&entry.offset), sizeof(entry.offset));
                file.read(reinterpret_cast<char*>(&entry.compressed_size), sizeof(entry.compressed_size));
                file.read(reinterpret_cast<char*>(&entry.original_size), sizeof(entry.original_size));

                entries[name] = entry;
            }

            pak_stream = std::move(file);
            return true;
        }

        std::vector<uint8_t> getFile(const std::string& filename)
        {
            printf("PakReader::get_file - %s\n", filename.c_str());
            auto it = entries.find(filename);
            if (it == entries.end())
            {
                printf("ERROR::PakReader::get_file - %s not found!\n", filename.c_str());
                throw std::runtime_error("File not found in pak");
            }

            pak_stream.seekg(it->second.offset, std::ios::beg);
            std::vector<uint8_t> compressed(it->second.compressed_size);
            pak_stream.read(reinterpret_cast<char*>(compressed.data()), compressed.size());

            EncryptionUtil::xorDecrypt(compressed, EncryptionUtil::deriveKey(passphrase));
            return decompressData(compressed, it->second.original_size);
        }

    private:
        const char* passphrase;

        std::ifstream pak_stream;
        std::unordered_map<std::string, FileEntry> entries;

        std::vector<uint8_t> decompressData(const std::vector<uint8_t>& compressed, size_t original_size)
        {
            std::vector<uint8_t> output(original_size);
            uLongf dest_len = original_size;
            if (uncompress(output.data(), &dest_len, compressed.data(), compressed.size()) != Z_OK)
            {
                throw std::runtime_error("Decompression failed");
            }
            return output;
        }
    };
}
