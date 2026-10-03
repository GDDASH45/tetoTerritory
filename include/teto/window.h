#ifndef TETOTERRITORY_WINDOW_H
#define TETOTERRITORY_WINDOW_H

typedef struct
{
    int x;
    int y;
    int width;
    int height;
    const char *title;
    int visible;
} teto_window_t;

teto_window_t *teto_window_create(
    int x,
    int y,
    int width,
    int height,
    const char *title
);

void teto_window_destroy(teto_window_t *window);

void teto_window_show(teto_window_t *window);
void teto_window_hide(teto_window_t *window);

void teto_window_draw(teto_window_t *window);

#endif