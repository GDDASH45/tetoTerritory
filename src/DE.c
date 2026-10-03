#include <DE.h>
#include <framebuffer.h>

void DE_init(void)
{
    framebuffer_init();
}

void DE_run(void)
{
}

void DE_shutdown(void)
{
    framebuffer_shutdown();
}