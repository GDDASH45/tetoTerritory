#include <ui/image.h>
#include <framebuffer.h>

#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <third-party/stb_image.h>

teto_image_t *image_load(
    const char *path
)
{
    teto_image_t *image;

    if (path == NULL)
        return NULL;

    image = malloc(
        sizeof(teto_image_t)
    );

    if (image == NULL)
        return NULL;

    image->pixels = stbi_load(
        path,
        &image->width,
        &image->height,
        &image->channels,
        4
    );

    if (image->pixels == NULL)
    {
        printf(
            "image_load: failed to load %s: %s\n",
            path,
            stbi_failure_reason()
        );

        free(image);
        return NULL;
    }

    image->channels = 4;

    return image;
}

void image_destroy(
    teto_image_t *image
)
{
    if (image == NULL)
        return;

    if (image->pixels != NULL)
        stbi_image_free(
            image->pixels
        );

    free(image);
}

void image_draw(
    teto_image_t *image,
    int x,
    int y
)
{
    int row;
    int column;

    if (image == NULL ||
        image->pixels == NULL)
        return;

    for (row = 0;
         row < image->height;
         row++)
    {
        for (column = 0;
             column < image->width;
             column++)
        {
            unsigned char *pixel;

            pixel =
                image->pixels +
                ((row * image->width + column) * 4);

            framebuffer_put_pixel(
                x + column,
                y + row,
                ((unsigned int)pixel[3] << 24) |
                ((unsigned int)pixel[2] << 16) |
                ((unsigned int)pixel[1] << 8) |
                (unsigned int)pixel[0]
            );
        }
    }
}