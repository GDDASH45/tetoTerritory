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
    teto_font_t *font;

    unsigned int background_color;

    int visible;

    unsigned int titlebar_color;
    int titlebar_height;

    unsigned int border_color;
    int border_width;
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

    window->border_color = 0xFFFFFFFF;
    window->border_width = 2;

    window->titlebar_color = 0xFF39C5BB;
    window->titlebar_height = 32;

    window->font = NULL;

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

void teto_window_set_font(
    teto_window_t *window,
    teto_font_t *font
)
{
    if (window == NULL)
        return;

    window->font = font;
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

void teto_window_set_border(
    teto_window_t *window,
    unsigned int color,
    int width
)
{
    if (window == NULL)
        return;

    if (width < 0)
        width = 1;

    window->border_color = color;
    window->border_width = width;
}

void teto_window_set_titlebar(
    teto_window_t *window,
    unsigned int color,
    int height
)
{
    if (window == NULL)
        return;

    if (height < 0)
        height = 0;

    window->titlebar_color = color;
    window->titlebar_height = height;
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
    int border;

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

    if (window->titlebar_height > 0)
    {
        framebuffer_fill_rect(
            window->x + window->border_width,
            window->y + window->border_width,
            window->width - window->border_width * 2,
            window->titlebar_height,
            window->titlebar_color
        );
    }

    if (window->font != NULL &&
        window->title != NULL &&
        window->titlebar_height > 0)
    {
        font_draw_text(
            window->font,
            window->x + window->border_width + 8,
            window->y + window->border_width + 23,
            window->title,
            0xFFFFFFFF
        );
    }

    for (border = 0;
         border < window->border_width;
         border++)
    {
        framebuffer_fill_rect(
            window->x + border,
            window->y + border,
            window->width - border * 2,
            1,
            window->border_color
        );

        framebuffer_fill_rect(
            window->x + border,
            window->y + window->height - border - 1,
            window->width - border * 2,
            1,
            window->border_color
        );

        framebuffer_fill_rect(
            window->x + border,
            window->y + border,
            1,
            window->height - border * 2,
            window->border_color
        );

        framebuffer_fill_rect(
            window->x + window->width - border - 1,
            window->y + border,
            1,
            window->height - border * 2,
            window->border_color
        );
    }
}