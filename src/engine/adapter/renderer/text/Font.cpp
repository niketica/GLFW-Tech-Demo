#include "engine/adapter/renderer/text/Font.h"

namespace niketica::renderer
{
    std::shared_ptr<Font> Font::load(const std::string& path, uint32_t pixelSize)
    {
        FT_Library ft = FreeTypeContext::get();

        auto loaded = asset::AssetManager::Get().Load<niketica::asset::File>(path);
        auto& bytes = loaded->fileData;
        auto data = bytes.data();
        auto size = bytes.size();
        FT_Face face;
        if (FT_New_Memory_Face(
            ft,
            reinterpret_cast<const FT_Byte*>(data),
            static_cast<FT_Long>(size),
            0,
            &face))
        {
            std::cerr << "ERROR::TextRenderer - Failed to load font from memory: "
                << path << std::endl;
            throw std::runtime_error("ERROR::TextRenderer - Failed to load font from memory: " + path);
        }

        FT_Set_Pixel_Sizes(face, 0, pixelSize);

        const uint32_t ATLAS_W = 1024;
        const uint32_t ATLAS_H = 1024;

        std::vector<uint8_t> atlasData(ATLAS_W * ATLAS_H, 0);

        uint32_t penX = 0, penY = 0, rowHeight = 0;

        auto font = std::make_shared<Font>();
        font->atlasWidth = ATLAS_W;
        font->atlasHeight = ATLAS_H;
        font->lineHeight = static_cast<float>(face->size->metrics.height >> 6);

        for (uint32_t c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                continue;

            FT_Bitmap& bmp = face->glyph->bitmap;

            if (penX + bmp.width >= ATLAS_W) {
                penX = 0;
                penY += rowHeight;
                rowHeight = 0;
            }

            for (uint32_t y = 0; y < bmp.rows; y++)
                for (uint32_t x = 0; x < bmp.width; x++)
                {
                    atlasData[(penX + x) + (penY + y) * ATLAS_W] =
                        bmp.buffer[x + y * bmp.width];
                }

            Glyph& g = font->glyphs[c];
            g.size = { bmp.width, bmp.rows };
            g.bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
            g.advance = face->glyph->advance.x;

            g.uvMin = {
                (float)penX / ATLAS_W,
                (float)penY / ATLAS_H
            };
            g.uvMax = {
                (float)(penX + bmp.width) / ATLAS_W,
                (float)(penY + bmp.rows) / ATLAS_H
            };

            penX += bmp.width + 1;
            rowHeight = std::max(rowHeight, bmp.rows);
        }

        glGenTextures(1, &font->atlasTexture);
        glBindTexture(GL_TEXTURE_2D, font->atlasTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ATLAS_W, ATLAS_H, 0,
            GL_RED, GL_UNSIGNED_BYTE, atlasData.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        FT_Done_Face(face);
        return font;
    }
}
