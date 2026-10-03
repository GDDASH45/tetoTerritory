#include <teto/window.h>
#include <framebuffer.h>

#include <stdlib.h>
#include <string.h>

struct teto_window
{
    int x;
    int y;
    int width;
    int height;

    char *title;

    unsigned int background_color;

    int visible;
};

teto_window_t *teto_window_create(
    int x,
    int y,
    int width,
    int height,
    const char *title
)
{
    teto_window_t *window;

    window = malloc(sizeof(teto_window_t));

    if (window == NULL)
        return NULL;

    window->x = x;
    window->y = y;
    window->width = width;
    window->height = height;
    window->visible = 1;
    window->title = NULL;
    window->background_color = 0xFF202020;

    if (title != NULL)
    {
        window->title = malloc(strlen(title) + 1);

        if (window->title == NULL)
        {
            free(window);
            return NULL;
        }

        strcpy(window->title, title);
    }

    return window;
}

void teto_window_destroy(
    teto_window_t *window
)
{
    if (window == NULL)
        return;

    free(window->title);
    free(window);
}

void teto_window_show(
    teto_window_t *window
)
{
    if (window != NULL)
        window->visible = 1;
}

void teto_window_hide(
    teto_window_t *window
)
{
    if (window != NULL)
        window->visible = 0;
}

void teto_window_set_position(
    teto_window_t *window,
    int x,
    int y
)
{
    if (window == NULL)
        return;

    window->x = x;
    window->y = y;
}

void teto_window_set_size(
    teto_window_t *window,
    int width,
    int height
)
{
    if (window == NULL)
        return;

    window->width = width;
    window->height = height;
}

void teto_window_set_title(
    teto_window_t *window,
    const char *title
)
{
    char *new_title;

    if (window == NULL)
        return;

    if (title == NULL)
    {
        free(window->title);
        window->title = NULL;
        return;
    }

    new_title = malloc(strlen(title) + 1);

    if (new_title == NULL)
        return;

    strcpy(new_title, title);

    free(window->title);
    window->title = new_title;
}

int teto_window_is_visible(
    const teto_window_t *window
)
{
    if (window == NULL)
        return 0;

    return window->visible;
}

void teto_window_set_background(
    teto_window_t *window,
    unsigned int color
)
{
    if (window == NULL)
        return;

    window->background_color = color;
}

void teto_window_draw(
    teto_window_t *window
)
{
    if (window == NULL)
        return;

    if (!window->visible)
        return;

    framebuffer_fill_rect(
        window->x,
        window->y,
        window->width,
        window->height,
        window->background_color
    );
}