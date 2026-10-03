#ifndef TETOTERRITORY_UI_FONT_H
#define TETOTERRITORY_UI_FONT_H

typedef struct teto_font teto_font_t;

teto_font_t *font_load(
    const char *path,
    int size
);

void font_destroy(
    teto_font_t *font
);

#endif