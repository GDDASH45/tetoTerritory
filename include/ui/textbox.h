#ifndef TETOTERRITORY_UI_TEXTBOX_H
#define TETOTERRITORY_UI_TEXTBOX_H

typedef struct
{
    int x;
    int y;
    int width;
    int height;

    char *text;
    int focused;
} teto_textbox_t;

teto_textbox_t *textbox_create(
    int x,
    int y,
    int width,
    int height
);

void textbox_destroy(teto_textbox_t *textbox);

void textbox_set_text(
    teto_textbox_t *textbox,
    const char *text
);

const char *textbox_get_text(
    const teto_textbox_t *textbox
);

void textbox_set_focus(
    teto_textbox_t *textbox,
    int focused
);

void textbox_draw(
    teto_textbox_t *textbox
);

#endif