#include <DE.h>
#include <panel.h>
#include <ui/font.h>

#include <stddef.h>

int main(void)
{
    teto_font_t *font;

    font = font_load(
        "/usr/share/tetoDE/teto.ttf",
        16
    );

    if (font == NULL)
        return 1;

    
    DE_init();
    panel_init();

    DE_run();

    panel_shutdown();
    DE_shutdown();

    font_destroy(font);

    return 0;
}