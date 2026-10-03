#include <mouse.h>

#include <framebuffer.h>

#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#define MOUSE_DEVICE "/dev/input/event2"

static int mouse_fd = -1;

static int mouse_position_x = 0;
static int mouse_position_y = 0;

static int mouse_button_left = 0;
static int mouse_button_middle = 0;
static int mouse_button_right = 0;

static void mouse_clamp_position(void)
{
    int width;
    int height;

    width = framebuffer_width();
    height = framebuffer_height();

    if (width <= 0 || height <= 0)
        return;

    if (mouse_position_x < 0)
        mouse_position_x = 0;

    if (mouse_position_y < 0)
        mouse_position_y = 0;

    if (mouse_position_x >= width)
        mouse_position_x = width - 1;

    if (mouse_position_y >= height)
        mouse_position_y = height - 1;
}

int mouse_init(void)
{
    mouse_fd = open(
        MOUSE_DEVICE,
        O_RDONLY | O_NONBLOCK
    );

    if (mouse_fd < 0)
        return -1;

    mouse_position_x = 0;
    mouse_position_y = 0;

    mouse_button_left = 0;
    mouse_button_middle = 0;
    mouse_button_right = 0;

    mouse_clamp_position();

    return 0;
}

void mouse_shutdown(void)
{
    if (mouse_fd >= 0)
    {
        close(mouse_fd);
        mouse_fd = -1;
    }
}

void mouse_update(void)
{
    struct input_event event;

    if (mouse_fd < 0)
        return;

    while (read(
        mouse_fd,
        &event,
        sizeof(event)
    ) == sizeof(event))
    {
        if (event.type == EV_REL)
        {
            if (event.code == REL_X)
                mouse_position_x += event.value;

            if (event.code == REL_Y)
                mouse_position_y += event.value;
        }

        if (event.type == EV_KEY)
        {
            if (event.code == BTN_LEFT)
                mouse_button_left = event.value;

            if (event.code == BTN_MIDDLE)
                mouse_button_middle = event.value;

            if (event.code == BTN_RIGHT)
                mouse_button_right = event.value;
        }
    }

    mouse_clamp_position();
}

int mouse_x(void)
{
    return mouse_position_x;
}

int mouse_y(void)
{
    return mouse_position_y;
}

int mouse_left(void)
{
    return mouse_button_left;
}

int mouse_middle(void)
{
    return mouse_button_middle;
}

int mouse_right(void)
{
    return mouse_button_right;
}