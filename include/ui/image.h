#ifndef TETOTERRITORY_UI_IMAGE_H
#define TETOTERRITORY_UI_IMAGE_H

typedef struct teto_image
{
    int width;
    int height;
    int channels;

    unsigned char *pixels;
} teto_image_t;

teto_image_t *image_load(
    const char *path
);

void image_destroy(
    teto_image_t *image
);

void image_draw(
    teto_image_t *image,
    int x,
    int y
);

#endif