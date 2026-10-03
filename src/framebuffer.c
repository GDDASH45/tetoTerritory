#include <framebuffer.h>

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

static int framebuffer_fd = -1;

static void *framebuffer = NULL;
static size_t framebuffer_size = 0;

static int framebuffer_screen_width = 0;
static int framebuffer_screen_height = 0;
static int framebuffer_screen_bpp = 0;
static int framebuffer_line_length = 0;

static struct fb_var_screeninfo framebuffer_var_info;

int framebuffer_init(void)
{
    struct fb_fix_screeninfo fix_info;
    struct fb_var_screeninfo var_info;

    framebuffer_fd = open(
        "/dev/fb0",
        O_RDWR
    );

    if (framebuffer_fd < 0)
        return -1;

    if (ioctl(
        framebuffer_fd,
        FBIOGET_FSCREENINFO,
        &fix_info
    ) < 0)
    {
        close(framebuffer_fd);
        framebuffer_fd = -1;

        return -1;
    }

    if (ioctl(
        framebuffer_fd,
        FBIOGET_VSCREENINFO,
        &var_info
    ) < 0)
    {
        close(framebuffer_fd);
        framebuffer_fd = -1;

        return -1;
    }

    framebuffer_var_info = var_info;

    framebuffer_screen_width =
        var_info.xres;

    framebuffer_screen_height =
        var_info.yres;

    framebuffer_screen_bpp =
        var_info.bits_per_pixel;

    framebuffer_line_length =
        fix_info.line_length;

    framebuffer_size =
        fix_info.smem_len;

    framebuffer = mmap(
        NULL,
        framebuffer_size,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        framebuffer_fd,
        0
    );

    if (framebuffer == MAP_FAILED)
    {
        framebuffer = NULL;

        close(framebuffer_fd);
        framebuffer_fd = -1;

        return -1;
    }

    return 0;
}

void framebuffer_shutdown(void)
{
    if (framebuffer != NULL)
    {
        munmap(
            framebuffer,
            framebuffer_size
        );

        framebuffer = NULL;
    }

    if (framebuffer_fd >= 0)
    {
        close(framebuffer_fd);
        framebuffer_fd = -1;
    }

    framebuffer_size = 0;

    framebuffer_screen_width = 0;
    framebuffer_screen_height = 0;
    framebuffer_screen_bpp = 0;
    framebuffer_line_length = 0;
}

int framebuffer_width(void)
{
    return framebuffer_screen_width;
}

int framebuffer_height(void)
{
    return framebuffer_screen_height;
}

int framebuffer_bpp(void)
{
    return framebuffer_screen_bpp;
}

void framebuffer_put_pixel(
    int x,
    int y,
    unsigned int color
)
{
    unsigned char *pixel;

    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;

    unsigned int value;

    if (framebuffer == NULL)
        return;

    if (x < 0 || y < 0)
        return;

    if (x >= framebuffer_screen_width ||
        y >= framebuffer_screen_height)
        return;

    if (framebuffer_screen_bpp != 32)
        return;

    red =
        (color >> 16) & 0xff;

    green =
        (color >> 8) & 0xff;

    blue =
        color & 0xff;

    alpha =
        (color >> 24) & 0xff;

    value = 0;

    if (framebuffer_var_info.red.length > 0)
    {
        red =
            red >>
            (8 - framebuffer_var_info.red.length);

        value |=
            red <<
            framebuffer_var_info.red.offset;
    }

    if (framebuffer_var_info.green.length > 0)
    {
        green =
            green >>
            (8 - framebuffer_var_info.green.length);

        value |=
            green <<
            framebuffer_var_info.green.offset;
    }

    if (framebuffer_var_info.blue.length > 0)
    {
        blue =
            blue >>
            (8 - framebuffer_var_info.blue.length);

        value |=
            blue <<
            framebuffer_var_info.blue.offset;
    }

    if (framebuffer_var_info.transp.length > 0)
    {
        alpha =
            alpha >>
            (8 - framebuffer_var_info.transp.length);

        value |=
            alpha <<
            framebuffer_var_info.transp.offset;
    }

    pixel =
        (unsigned char *)framebuffer +
        (y * framebuffer_line_length) +
        (x * 4);

    *(unsigned int *)pixel = value;
}

unsigned int framebuffer_get_pixel(
    int x,
    int y
)
{
    unsigned char *pixel;

    unsigned int value;

    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;

    unsigned int red_max;
    unsigned int green_max;
    unsigned int blue_max;
    unsigned int alpha_max;

    if (framebuffer == NULL)
        return 0;

    if (x < 0 || y < 0)
        return 0;

    if (x >= framebuffer_screen_width ||
        y >= framebuffer_screen_height)
        return 0;

    if (framebuffer_screen_bpp != 32)
        return 0;

    pixel =
        (unsigned char *)framebuffer +
        (y * framebuffer_line_length) +
        (x * 4);

    value = *(unsigned int *)pixel;

    red = 0;
    green = 0;
    blue = 0;
    alpha = 255;

    if (framebuffer_var_info.red.length > 0)
    {
        red_max =
            (1U << framebuffer_var_info.red.length) - 1;

        red =
            (value >> framebuffer_var_info.red.offset) &
            red_max;

        red =
            (red * 255) /
            red_max;
    }

    if (framebuffer_var_info.green.length > 0)
    {
        green_max =
            (1U << framebuffer_var_info.green.length) - 1;

        green =
            (value >> framebuffer_var_info.green.offset) &
            green_max;

        green =
            (green * 255) /
            green_max;
    }

    if (framebuffer_var_info.blue.length > 0)
    {
        blue_max =
            (1U << framebuffer_var_info.blue.length) - 1;

        blue =
            (value >> framebuffer_var_info.blue.offset) &
            blue_max;

        blue =
            (blue * 255) /
            blue_max;
    }

    if (framebuffer_var_info.transp.length > 0)
    {
        alpha_max =
            (1U << framebuffer_var_info.transp.length) - 1;

        alpha =
            (value >> framebuffer_var_info.transp.offset) &
            alpha_max;

        alpha =
            (alpha * 255) /
            alpha_max;
    }

    return
        (alpha << 24) |
        (red << 16) |
        (green << 8) |
        blue;
}

void framebuffer_fill_rect(
    int x,
    int y,
    int width,
    int height,
    unsigned int color
)
{
    int current_x;
    int current_y;

    if (width <= 0 || height <= 0)
        return;

    for (current_y = y;
         current_y < y + height;
         current_y++)
    {
        for (current_x = x;
             current_x < x + width;
             current_x++)
        {
            framebuffer_put_pixel(
                current_x,
                current_y,
                color
            );
        }
    }
}