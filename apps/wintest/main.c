#include <teto/window.h>
#include <framebuffer.h>

#include <unistd.h>

int main(void)
{
    teto_font_t *font;
    teto_window_t *window;

    if (framebuffer_init() < 0)
        return 1;

    font = font_load(
        "/usr/share/fonts/truetype/ubuntu/UbuntuMono-RI.ttf",
        16
    );

    if (font == NULL)
        return 1;

    window = teto_window_create(
        100,
        100,
        800,
        500,
        "window"
    );

    if (window == NULL)
    {
        framebuffer_shutdown();
        font_destroy(font);
        return 1;
    }

    teto_window_set_background(
        window,
        0xFF303030
    );

    teto_window_set_border(
        window,
        0xFFFFFFFF,
        2
    );

    teto_window_set_font(
        window,
        font
    );

    teto_window_set_titlebar(
        window,
        0xFF39C5BB,
        32
    );

    teto_window_draw(window);

    while (1)
        sleep(1);

    teto_window_destroy(window);
    font_destroy(font);
    framebuffer_shutdown();

    return 0;
}