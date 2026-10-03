#include <DE.h>
#include <framebuffer.h>

#include <fcntl.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define TETO_TTY "/dev/tty13"
#define TETO_VT  13

static int console_fd = -1;

void DE_init(void)
{
    console_fd = open(
        TETO_TTY,
        O_RDWR
    );

    if (console_fd >= 0)
    {
        ioctl(
            console_fd,
            VT_ACTIVATE,
            TETO_VT
        );

        ioctl(
            console_fd,
            VT_WAITACTIVE,
            TETO_VT
        );

        write(
            console_fd,
            "\033[2J\033[H",
            7
        );

        ioctl(
            console_fd,
            KDSETMODE,
            KD_GRAPHICS
        );
    }

    framebuffer_init();
}

void DE_run(void)
{
}

void DE_shutdown(void)
{
    if (console_fd >= 0)
    {
        ioctl(
            console_fd,
            KDSETMODE,
            KD_TEXT
        );

        write(
            console_fd,
            "\033[2J\033[H",
            7
        );

        close(
            console_fd
        );

        console_fd = -1;
    }

    framebuffer_shutdown();
}