#include <ui/font.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <framebuffer.h>

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

    if (FT_New_Face(
        font->library,
        path,
        0,
        &font->face
    ))
    {
        FT_Done_FreeType(font->library);
        free(font);
        return NULL;
    }

    if (FT_Set_Pixel_Sizes(
        font->face,
        0,
        size
    ))
    {
        FT_Done_Face(font->face);
        FT_Done_FreeType(font->library);
        free(font);
        return NULL;
    }

    font->size = size;

    return font;
}

void font_destroy(
    teto_font_t *font
)
{
    if (font == NULL)
        return;

    FT_Done_Face(font->face);
    FT_Done_FreeType(font->library);

    free(font);
}

void font_draw_text(
    teto_font_t *font,
    int x,
    int y,
    const char *text,
    unsigned int color
)
{
    int pen_x;

    if (font == NULL || text == NULL)
        return;

    pen_x = x;

    while (*text != '\0')
    {
        FT_GlyphSlot glyph;
        int glyph_x;
        int glyph_y;
        int row;
        int column;

        if (FT_Load_Char(
            font->face,
            (unsigned char)*text,
            FT_LOAD_RENDER
        ))
        {
            text++;
            continue;
        }

        glyph = font->face->glyph;

        glyph_x = pen_x + glyph->bitmap_left;
        glyph_y = y - glyph->bitmap_top;

        for (row = 0;
             row < (int)glyph->bitmap.rows;
             row++)
        {
            for (column = 0;
                 column < (int)glyph->bitmap.width;
                 column++)
            {
                unsigned char alpha;

                alpha = glyph->bitmap.buffer[
                    row * glyph->bitmap.pitch + column
                ];

                if (alpha == 0)
                    continue;

                framebuffer_put_pixel(
                    glyph_x + column,
                    glyph_y + row,
                    color
                );
            }
        }

        pen_x += glyph->advance.x >> 6;

        text++;
    }
}