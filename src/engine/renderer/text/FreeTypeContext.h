#pragma once

#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace niketica::renderer
{
    class FreeTypeContext {
    public:
        static FT_Library get()
        {
            static FT_Library library = [] {
                FT_Library lib;
                if (FT_Init_FreeType(&lib))
                {
                    throw std::runtime_error("FreeType init failed");
                }
                return lib;
                }();
            return library;
        }

        static void release()
        {
            FT_Library library = get();
            FT_Done_FreeType(library);
        }
    };
}