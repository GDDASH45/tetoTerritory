#include <framebuffer.h>

#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

static int framebuffer_fd = -1;
static void *framebuffer = NULL;
static size_t framebuffer_size = 0;

int framebuffer_init(void)
{
    struct fb_fix_screeninfo fix_info;
    struct fb_var_screeninfo var_info;

    framebuffer_fd = open("/dev/fb0", O_RDWR);

    if (framebuffer_fd < 0)
        return -1;

    if (ioctl(framebuffer_fd, FBIOGET_FSCREENINFO, &fix_info) < 0)
    {
        close(framebuffer_fd);
        framebuffer_fd = -1;
        return -1;
    }

    if (ioctl(framebuffer_fd, FBIOGET_VSCREENINFO, &var_info) < 0)
    {
        close(framebuffer_fd);
        framebuffer_fd = -1;
        return -1;
    }

    framebuffer_size = fix_info.smem_len;

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
        munmap(framebuffer, framebuffer_size);
        framebuffer = NULL;
    }

    if (framebuffer_fd >= 0)
    {
        close(framebuffer_fd);
        framebuffer_fd = -1;
    }

    framebuffer_size = 0;
}