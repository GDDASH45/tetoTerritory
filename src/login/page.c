#include <login/page.h>

#include <stdlib.h>

typedef struct
{
    int visible;
} login_page_t;

static login_page_t login_page;

void login_page_init(void)
{
    login_page.visible = 1;
}

void login_page_show(void)
{
    login_page.visible = 1;
}

void login_page_hide(void)
{
    login_page.visible = 0;
}

int login_page_visible(void)
{
    return login_page.visible;
}

void login_page_shutdown(void)
{
    login_page.visible = 0;
}