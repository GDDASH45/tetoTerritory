#include <teto/window.h>
#include <framebuffer.h>

#include <unistd.h>

int main(void)
{
    teto_window_t *window;

    if (framebuffer_init() < 0)
        return 1;

    window = teto_window_create(
        100,
        100,
        800,
        500,
        "Window Test"
    );

    if (window == NULL)
    {
        framebuffer_shutdown();
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

    teto_window_set_titlebar(
        window,
        0xFF39C5BB,
        32
    );

    teto_window_draw(window);

    while (1)
        sleep(1);

    teto_window_destroy(window);
    framebuffer_shutdown();

    return 0;
}