#include "engine/asset/PakReader.h"

namespace niketica::asset
{
    bool PakReader::load(const std::string& pak_file)
    {
        if (loaded)
        {
            return true;
        }

        std::cout << "INFO::PakReader::load - path=" << pak_file << std::endl;

        pak_stream.open(pak_file, std::ios::binary);
        if (!pak_stream.is_open())
        {
            return false;
        }

        uint32_t file_count = read_u32_le(pak_stream);

        for (uint32_t i = 0; i < file_count; ++i)
        {
            uint32_t name_len;
            pak_stream.read(reinterpret_cast<char*>(&name_len), sizeof(name_len));
            std::string name(name_len, '\0');
            pak_stream.read(&name[0], name_len);

            FileEntry entry;
            entry.offset = read_u32_le(pak_stream);
            entry.compressed_size = read_u32_le(pak_stream);
            entry.original_size = read_u32_le(pak_stream);

            entries[name] = entry;
        }

        loaded = true;
        return true;
    }

    bool PakReader::hasFile(const std::string& filename) const {
        return entries.find(filename) != entries.end();
    }

    std::vector<uint8_t> PakReader::getFile(const std::string& filename)
    {
        auto it = entries.find(filename);
        if (it == entries.end())
        {
            printf("ERROR::PakReader::getFile - %s not found!\n", filename.c_str());

            std::cout << entries.size() << " available files:" << std::endl;
            for (const auto& pair : entries) {
                std::cout << " - " << pair.first << std::endl;
            }

            throw std::runtime_error("File not found in pak");
        }

        pak_stream.seekg(it->second.offset, std::ios::beg);
        std::vector<uint8_t> compressed(it->second.compressed_size);
        pak_stream.read(reinterpret_cast<char*>(compressed.data()), compressed.size());

        EncryptionUtil::xorDecrypt(compressed, EncryptionUtil::deriveKey(passphrase));
        return decompressData(compressed, it->second.original_size);
    }

    std::vector<uint8_t> PakReader::decompressData(const std::vector<uint8_t>& compressed, size_t original_size)
    {
        std::vector<uint8_t> output(original_size);
        uLongf dest_len = (uLongf)original_size;
        if (uncompress(output.data(), &dest_len, compressed.data(), (uLongf)compressed.size()) != Z_OK)
        {
            throw std::runtime_error("Decompression failed");
        }
        return output;
    }

    uint32_t PakReader::read_u32_le(std::istream& in) {
        uint8_t b[4];
        in.read(reinterpret_cast<char*>(b), 4);
        return (uint32_t)b[0]
            | (uint32_t)b[1] << 8
            | (uint32_t)b[2] << 16
            | (uint32_t)b[3] << 24;
    }

}
