#include <startmenu.h>

#include <framebuffer.h>
#include <mouse.h>
#include <ui/image.h>

#define START_BUTTON_PATH "/sbin/assets/start-button.bmp"

static teto_image_t *start_button = NULL;

static int startmenu_open = 0;

static int start_button_x = 0;
static int start_button_y = 0;

static int previous_mouse_left = 0;

int startmenu_init(void)
{
    start_button = image_load(
        START_BUTTON_PATH
    );

    if (start_button == NULL)
        return -1;

    return 0;
}

void startmenu_shutdown(void)
{
    if (start_button != NULL)
    {
        image_destroy(
            start_button
        );

        start_button = NULL;
    }
}

void startmenu_update(void)
{
    int x;
    int y;
    int left;

    if (start_button == NULL)
        return;

    x = mouse_x();
    y = mouse_y();

    left = mouse_left();

    if (left && !previous_mouse_left)
    {
        if (x >= start_button_x &&
            x < start_button_x + start_button->width &&
            y >= start_button_y &&
            y < start_button_y + start_button->height)
        {
            startmenu_open =
                !startmenu_open;
        }
    }

    previous_mouse_left = left;
}

void startmenu_draw(void)
{
    int height;

    if (start_button == NULL)
        return;

    height = framebuffer_height();

    start_button_x = 0;

    start_button_y =
        height - start_button->height;

    if (startmenu_open)
    {
        framebuffer_fill_rect(
            0,
            height - 500,
            400,
            500,
            0xFF202020
        );
    }

    image_draw(
        start_button,
        start_button_x,
        start_button_y
    );
}

int startmenu_is_open(void)
{
    return startmenu_open;
}