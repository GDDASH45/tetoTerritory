#include <DE.h>
#include <panel.h>
#include <ui/font.h>
#include <mouse.h>

#include <stddef.h>

int main(void)
{
    teto_font_t *font;

    DE_init();

    if (mouse_init() != 0)
    {
        DE_shutdown();
        return 1;
    }

    /*
    font = font_load(
        "/usr/share/tetoDE/teto.ttf",
        16
    );

    if (font == NULL)
    {
        mouse_shutdown();
        DE_shutdown();
        return 1;
    }
    */

    panel_init();

    while (1)
    {
        mouse_update();

        DE_run();

        mouse_draw();
    }

    panel_shutdown();

    mouse_shutdown();

    DE_shutdown();

    //font_destroy(font);

    return 0;
}