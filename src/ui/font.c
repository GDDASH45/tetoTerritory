#include <ui/font.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdlib.h>

struct teto_font
{
    FT_Library library;
    FT_Face face;
    int size;
};

teto_font_t *font_load(
    const char *path,
    int size
)
{
    teto_font_t *font;

    if (path == NULL || size <= 0)
        return NULL;

    font = malloc(sizeof(teto_font_t));

    if (font == NULL)
        return NULL;

    if (FT_Init_FreeType(&font->library))
    {
        free(font);
        return NULL;
    }

    if (FT_New_Face(font->library, path, 0, &font->face))
    {
        FT_Done_FreeType(font->library);
        free(font);
        return NULL;
    }

    if (FT_Set_Pixel_Sizes(font->face, 0, size))
    {
        FT_Done_Face(font->face);
        FT_Done_FreeType(font->library);
        free(font);
        return NULL;
    }

    font->size = size;

    return font;
}

void font_destroy(teto_font_t *font)
{
    if (font == NULL)
        return;

    FT_Done_Face(font->face);
    FT_Done_FreeType(font->library);

    free(font);
}