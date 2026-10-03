#include <mouse.h>
#include <framebuffer.h>

#define MOUSE_CURSOR_WIDTH  16
#define MOUSE_CURSOR_HEIGHT 16

static unsigned int mouse_background[
    MOUSE_CURSOR_HEIGHT
][MOUSE_CURSOR_WIDTH];

static int mouse_background_x = 0;
static int mouse_background_y = 0;
static int mouse_background_valid = 0;

static const unsigned char cursor[
    MOUSE_CURSOR_HEIGHT
][MOUSE_CURSOR_WIDTH] =
{
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0},
    {1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0},
    {1,2,2,2,2,2,2,2,2,2,2,1,0,0,0,0},
    {1,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1}
};

static void mouse_restore_background(void)
{
    int row;
    int column;

    if (!mouse_background_valid)
        return;

    for (row = 0; row < MOUSE_CURSOR_HEIGHT; row++)
    {
        for (column = 0; column < MOUSE_CURSOR_WIDTH; column++)
        {
            framebuffer_put_pixel(
                mouse_background_x + column,
                mouse_background_y + row,
                mouse_background[row][column]
            );
        }
    }

    mouse_background_valid = 0;
}

static void mouse_save_background(
    int x,
    int y
)
{
    int row;
    int column;

    for (row = 0; row < MOUSE_CURSOR_HEIGHT; row++)
    {
        for (column = 0; column < MOUSE_CURSOR_WIDTH; column++)
        {
            mouse_background[row][column] =
                framebuffer_get_pixel(
                    x + column,
                    y + row
                );
        }
    }

    mouse_background_x = x;
    mouse_background_y = y;
    mouse_background_valid = 1;
}

static void mouse_draw_cursor(
    int x,
    int y
)
{
    int row;
    int column;

    for (row = 0; row < MOUSE_CURSOR_HEIGHT; row++)
    {
        for (column = 0; column < MOUSE_CURSOR_WIDTH; column++)
        {
            if (cursor[row][column] == 1)
            {
                framebuffer_put_pixel(
                    x + column,
                    y + row,
                    0xFF000000
                );
            }
            else if (cursor[row][column] == 2)
            {
                framebuffer_put_pixel(
                    x + column,
                    y + row,
                    0xFFFFFFFF
                );
            }
        }
    }
}

void mouse_draw(void)
{
    int x;
    int y;

    mouse_restore_background();

    x = mouse_x();
    y = mouse_y();

    mouse_save_background(
        x,
        y
    );

    mouse_draw_cursor(
        x,
        y
    );
}