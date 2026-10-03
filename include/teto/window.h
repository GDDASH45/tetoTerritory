#ifndef TETOTERRITORY_WINDOW_H
#define TETOTERRITORY_WINDOW_H

typedef struct teto_window teto_window_t;

#include <ui/font.h>

teto_window_t *teto_window_create(
    int x,
    int y,
    int width,
    int height,
    const char *title
);

void teto_window_destroy(
    teto_window_t *window
);

void teto_window_show(
    teto_window_t *window
);

void teto_window_hide(
    teto_window_t *window
);

void teto_window_set_position(
    teto_window_t *window,
    int x,
    int y
);

void teto_window_set_size(
    teto_window_t *window,
    int width,
    int height
);

void teto_window_set_title(
    teto_window_t *window,
    const char *title
);

void teto_window_set_background(
    teto_window_t *window,
    unsigned int color
);

int teto_window_is_visible(
    const teto_window_t *window
);

void teto_window_set_border(
    teto_window_t *window,
    unsigned int color,
    int width
);

void teto_window_set_titlebar(
    teto_window_t *window,
    unsigned int color,
    int height
);

void teto_window_set_font(
    teto_window_t *window,
    teto_font_t *font
);

void teto_window_draw(
    teto_window_t *window
);

#endif