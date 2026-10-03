#include <teto/window.h>

#include <stdlib.h>
#include <string.h>

teto_window_t *teto_window_create(
    int x,
    int y,
    int width,
    int height,
    const char *title
)
{
    teto_window_t *window = malloc(sizeof(teto_window_t));

    if (window == NULL)
        return NULL;

    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    window->visible = 1;

    if (title != NULL)
    {
        size_t length = strlen(title) + 1;

        window->title = malloc(length);

        if (window->title == NULL)
        {
            free(window);
            return NULL;
        }

        memcpy((char *)window->title, title, length);
    }
    else
    {
        window->title = NULL;
    }

    return window;
}

void teto_window_destroy(teto_window_t *window)
{
    if (window == NULL)
        return;

    free((char *)window->title);
    free(window);
}

void teto_window_show(teto_window_t *window)
{
    if (window != NULL)
        window->visible = 1;
}

void teto_window_hide(teto_window_t *window)
{
    if (window != NULL)
        window->visible = 0;
}

void teto_window_draw(teto_window_t *window)
{
    if (window == NULL || !window->visible)
        return;
}