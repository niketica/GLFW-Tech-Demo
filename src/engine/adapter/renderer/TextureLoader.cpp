#include "engine/adapter/renderer/TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace niketica::renderer
{
    TextureLoader::TextureLoader()
    {
        for (unsigned int i = 0; i < MAX_NR_OF_TEXTURES; ++i)
        {
            textureIDs.push_back(i);
        }
    }

    TextureLoader::~TextureLoader()
    {
        for (const auto& pair : textureIDMap)
        {
            glDeleteTextures(1, &pair.second.glTextureID);
        }
    }

    component::TextureHandle TextureLoader::acquire(const char* path, bool compressed)
    {
        if (pathToTextureIDMap.find(path) != pathToTextureIDMap.end())
        {
            unsigned int existingTextureID = pathToTextureIDMap[path];
            return { existingTextureID };
        }

        GLuint glTextureID;
        if (compressed)
        {
            glTextureID = loadCompressedTexture(path);
        }
        else
        {
            glTextureID = loadUncompressedTexture(path);
        }
        if (glTextureID == 0)
        {
            std::cerr << "ERROR::TextureLoader::acquire - Failed to load texture: " << path << std::endl;
            return component::TextureHandle();
        }

        return addTexture(path, glTextureID);
    }

    component::TextureHandle TextureLoader::addTexture(const char* path, unsigned int glTextureID)
    {
        if (textureIDs.empty())
        {
            std::cerr << "ERROR::TextureLoader::addTexture - Maximum number of textures reached!" << std::endl;
            return component::TextureHandle();
        }

        unsigned int textureID = textureIDs.back();
        textureIDs.pop_back();

        textureIDMap[textureID] = { glTextureID, std::string(path) };
        pathToTextureIDMap[path] = textureID;

        return { textureID };
    }

    void TextureLoader::release(const component::TextureHandle& textureHandle)
    {
        auto it = textureIDMap.find(textureHandle.id);
        if (it != textureIDMap.end())
        {
            glDeleteTextures(1, &it->second.glTextureID);
            pathToTextureIDMap.erase(it->second.path);
            textureIDMap.erase(it);
            textureIDs.push_back(textureHandle.id);
        }
        else
        {
            std::cerr << "ERROR::TextureLoader::release - Texture ID not found: " << textureHandle.id << std::endl;
        }
    }

    unsigned int TextureLoader::loadUncompressedTexture(const char* path)
    {
        unsigned int textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture and generate mipmaps
        int width;
        int height;
        int nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cerr << "ERROR::TextureLoader::loadTexture - Failed to load texture, check the file format: " << path << std::endl;
        }
        stbi_image_free(data);

        return textureID;
    }

    unsigned int TextureLoader::loadCompressedTexture(const char* path)
    {
        auto loaded = niketica::asset::AssetManager::Get().Load<niketica::asset::File>(path);
        auto& bytes = loaded->fileData;
        return loadCompressedTextureFromMemory(bytes.data(), bytes.size(), path);
    }


    // GLuint TextureLoader::getGLTextureID(unsigned int textureID)
    // {
    //     auto it = textureIDMap.find(textureID);
    //     if (it != textureIDMap.end())
    //     {
    //         return it->second.glTextureID;
    //     }
    //     std::cerr << "ERROR::TextureLoader::getGLTextureID - Texture ID not found: " << textureID << std::endl;
    //     return 0;
    // }
    unsigned int TextureLoader::getAdapterTextureID(unsigned int textureID)
    {
        auto it = textureIDMap.find(textureID);
        if (it != textureIDMap.end())
        {
            return it->second.glTextureID;
        }
        std::cerr << "ERROR::TextureLoader::getGLTextureID - Texture ID not found: " << textureID << std::endl;
        return 0;
    }

    unsigned int TextureLoader::loadCompressedTextureFromMemory(
        const uint8_t* data,
        size_t dataSize,
        const char* debugName)
    {
        if (!GLAD_GL_EXT_texture_compression_s3tc)
        {
            std::cerr << "ERROR::TextureLoader - S3TC not supported!" << std::endl;
            return 0;
        }

        MemoryReader reader(data, dataSize);

        // --- Validate magic ---
        char filecode[4];
        if (!reader.readBytes(filecode, 4) || std::strncmp(filecode, "DDS ", 4) != 0)
        {
            std::cerr << "ERROR::TextureLoader - Not a DDS file: "
                << (debugName ? debugName : "<memory>") << std::endl;
            return 0;
        }

        static_assert(sizeof(DDS_HEADER) == 124, "DDS header size mismatch");

        DDS_HEADER header;
        if (!reader.read(header))
        {
            std::cerr << "ERROR::TextureLoader - DDS header truncated" << std::endl;
            return 0;
        }

        uint32_t width = header.width;
        uint32_t height = header.height;
        uint32_t mipMapCount = header.mipMapCount ? header.mipMapCount : 1;

        GLenum format;
        uint32_t blockSize;

        switch (header.ddspf.fourCC)
        {
        case 0x31545844: // DXT1
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            blockSize = 8;
            break;
        case 0x33545844: // DXT3
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            blockSize = 16;
            break;
        case 0x35545844: // DXT5
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            blockSize = 16;
            break;
        default:
            std::cerr << "ERROR::TextureLoader - Unsupported DDS format" << std::endl;
            return 0;
        }

        // --- Create OpenGL texture ---
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            mipMapCount > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        uint32_t w = width;
        uint32_t h = height;

        for (uint32_t level = 0; level < mipMapCount; ++level)
        {
            uint32_t size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;

            if (reader.ptr + size > reader.end)
            {
                std::cerr << "ERROR::TextureLoader - DDS mip data truncated" << std::endl;
                glDeleteTextures(1, &texID);
                return 0;
            }

            glCompressedTexImage2D(
                GL_TEXTURE_2D,
                level,
                format,
                w,
                h,
                0,
                size,
                reader.ptr
            );

            reader.ptr += size;
            w = std::max(1u, w / 2);
            h = std::max(1u, h / 2);
        }

        return texID;
    }

    bool TextureLoader::uploadDDSIntoArrayLayer(
        TextureArray& array,
        uint16_t layer,
        const uint8_t* data,
        size_t dataSize)
    {
        MemoryReader reader(data, dataSize);

        char magic[4];
        if (!reader.readBytes(magic, 4) || std::strncmp(magic, "DDS ", 4) != 0)
            return false;

        DDS_HEADER header;
        if (!reader.read(header))
            return false;

        GLenum format;
        uint32_t blockSize;

        switch (header.ddspf.fourCC)
        {
        case 0x31545844: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; blockSize = 8; break;
        case 0x33545844: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; blockSize = 16; break;
        case 0x35545844: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blockSize = 16; break;
        default:
            return false;
        }

        glBindTexture(GL_TEXTURE_2D_ARRAY, array.glID);

        uint32_t mipCount =
            std::min(header.mipMapCount ? header.mipMapCount : 1,
                array.mipLevels);

        uint32_t w = header.width;
        uint32_t h = header.height;

        for (uint32_t mip = 0; mip < mipCount; ++mip)
        {
            uint32_t size =
                ((w + 3) / 4) * ((h + 3) / 4) * blockSize;

            glCompressedTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                mip,
                0, 0, layer,
                w, h, 1,
                format,
                size,
                reader.ptr
            );

            GLenum err = glGetError();
            if (err != GL_NO_ERROR)
            {
                std::cerr << "Upload failed at mip " << mip << "\n";
                return false;
            }

            reader.ptr += size;
            w = std::max(1u, w >> 1);
            h = std::max(1u, h >> 1);
        }

        nrOfDDSUploadedIntoArray++;

        return true;
    }

    bool TextureLoader::uploadDDSIntoArrayLayer(
        GLuint glTextureArrayID,
        uint32_t mipLevels,
        uint16_t layer,
        const uint8_t* data,
        size_t dataSize)
    {
        MemoryReader reader(data, dataSize);

        char magic[4];
        if (!reader.readBytes(magic, 4) || std::strncmp(magic, "DDS ", 4) != 0)
            return false;

        DDS_HEADER header;
        if (!reader.read(header))
            return false;

        GLenum format;
        uint32_t blockSize;

        switch (header.ddspf.fourCC)
        {
        case 0x31545844: format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; blockSize = 8; break;
        case 0x33545844: format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; blockSize = 16; break;
        case 0x35545844: format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; blockSize = 16; break;
        default:
            return false;
        }

        glBindTexture(GL_TEXTURE_2D_ARRAY, glTextureArrayID);

        uint32_t mipCount =
            std::min(header.mipMapCount ? header.mipMapCount : 1,
                mipLevels);

        uint32_t w = header.width;
        uint32_t h = header.height;

        for (uint32_t mip = 0; mip < mipCount; ++mip)
        {
            uint32_t size =
                ((w + 3) / 4) * ((h + 3) / 4) * blockSize;

            glCompressedTexSubImage3D(
                GL_TEXTURE_2D_ARRAY,
                mip,
                0, 0, layer,
                w, h, 1,
                format,
                size,
                reader.ptr
            );

            GLenum err = glGetError();
            if (err != GL_NO_ERROR)
            {
                std::cerr << "Upload failed at mip " << mip << "\n";
                return false;
            }

            reader.ptr += size;
            w = std::max(1u, w >> 1);
            h = std::max(1u, h >> 1);
        }

        nrOfDDSUploadedIntoArray++;

        return true;
    }
}
