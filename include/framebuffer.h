#ifndef TETOTERRITORY_FRAMEBUFFER_H
#define TETOTERRITORY_FRAMEBUFFER_H

#include <stddef.h>

int framebuffer_init(void);
void framebuffer_shutdown(void);

int framebuffer_width(void);
int framebuffer_height(void);
int framebuffer_bpp(void);

void framebuffer_put_pixel(
    int x,
    int y,
    unsigned int color
);

void framebuffer_fill_rect(
    int x,
    int y,
    int width,
    int height,
    unsigned int color
);

unsigned int framebuffer_get_pixel(
    int x,
    int y
);

#endif