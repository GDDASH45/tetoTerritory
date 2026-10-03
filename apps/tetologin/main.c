#include <framebuffer.h>
#include <ui/font.h>

#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define KEYBOARD_DEVICE "/dev/input/event1"
#define TETODE_PATH      "/sbin/tetode"

#define LOGIN_USERNAME "user"
#define LOGIN_PASSWORD "password"

#define LOGIN_FONT_PATH "/usr/share/tetoDE/UbuntuMono-RI.ttf"

#define COLOR_BACKGROUND 0xFF202020
#define COLOR_PANEL      0xFF303030
#define COLOR_FIELD      0xFF202020
#define COLOR_BORDER     0xFF505050
#define COLOR_TEXT       0xFFFFFFFF
#define COLOR_MUTED      0xFFAAAAAA
#define COLOR_ACCENT     0xFF39C5BB

#define INPUT_MAX 127

static int keyboard_fd = -1;

static char username[INPUT_MAX + 1];
static char password[INPUT_MAX + 1];

static int username_length = 0;
static int password_length = 0;

static int active_field = 0;

static int shift_pressed = 0;

static teto_font_t *font = NULL;

static char key_to_char(
    unsigned short code,
    int shift
)
{
    switch (code)
    {
        case KEY_A: return shift ? 'A' : 'a';
        case KEY_B: return shift ? 'B' : 'b';
        case KEY_C: return shift ? 'C' : 'c';
        case KEY_D: return shift ? 'D' : 'd';
        case KEY_E: return shift ? 'E' : 'e';
        case KEY_F: return shift ? 'F' : 'f';
        case KEY_G: return shift ? 'G' : 'g';
        case KEY_H: return shift ? 'H' : 'h';
        case KEY_I: return shift ? 'I' : 'i';
        case KEY_J: return shift ? 'J' : 'j';
        case KEY_K: return shift ? 'K' : 'k';
        case KEY_L: return shift ? 'L' : 'l';
        case KEY_M: return shift ? 'M' : 'm';
        case KEY_N: return shift ? 'N' : 'n';
        case KEY_O: return shift ? 'O' : 'o';
        case KEY_P: return shift ? 'P' : 'p';
        case KEY_Q: return shift ? 'Q' : 'q';
        case KEY_R: return shift ? 'R' : 'r';
        case KEY_S: return shift ? 'S' : 's';
        case KEY_T: return shift ? 'T' : 't';
        case KEY_U: return shift ? 'U' : 'u';
        case KEY_V: return shift ? 'V' : 'v';
        case KEY_W: return shift ? 'W' : 'w';
        case KEY_X: return shift ? 'X' : 'x';
        case KEY_Y: return shift ? 'Y' : 'y';
        case KEY_Z: return shift ? 'Z' : 'z';

        case KEY_0: return shift ? ')' : '0';
        case KEY_1: return shift ? '!' : '1';
        case KEY_2: return shift ? '@' : '2';
        case KEY_3: return shift ? '#' : '3';
        case KEY_4: return shift ? '$' : '4';
        case KEY_5: return shift ? '%' : '5';
        case KEY_6: return shift ? '^' : '6';
        case KEY_7: return shift ? '&' : '7';
        case KEY_8: return shift ? '*' : '8';
        case KEY_9: return shift ? '(' : '9';

        case KEY_SPACE:
            return ' ';

        case KEY_MINUS:
            return shift ? '_' : '-';

        case KEY_EQUAL:
            return shift ? '+' : '=';

        case KEY_DOT:
            return shift ? '>' : '.';

        case KEY_COMMA:
            return shift ? '<' : ',';

        case KEY_SLASH:
            return shift ? '?' : '/';

        case KEY_SEMICOLON:
            return shift ? ':' : ';';

        case KEY_APOSTROPHE:
            return shift ? '"' : '\'';

        case KEY_LEFTBRACE:
            return shift ? '{' : '[';

        case KEY_RIGHTBRACE:
            return shift ? '}' : ']';

        case KEY_BACKSLASH:
            return shift ? '|' : '\\';

        case KEY_GRAVE:
            return shift ? '~' : '`';

        default:
            return 0;
    }
}

static void input_backspace(void)
{
    if (active_field == 0)
    {
        if (username_length > 0)
        {
            username_length--;
            username[username_length] = '\0';
        }
    }
    else
    {
        if (password_length > 0)
        {
            password_length--;
            password[password_length] = '\0';
        }
    }
}

static void input_character(
    char character
)
{
    if (active_field == 0)
    {
        if (username_length >= INPUT_MAX)
            return;

        username[username_length++] = character;
        username[username_length] = '\0';
    }
    else
    {
        if (password_length >= INPUT_MAX)
            return;

        password[password_length++] = character;
        password[password_length] = '\0';
    }
}

static int login_check(void)
{
    if (strcmp(
        username,
        LOGIN_USERNAME
    ) != 0)
    {
        return 0;
    }

    if (strcmp(
        password,
        LOGIN_PASSWORD
    ) != 0)
    {
        return 0;
    }

    return 1;
}

static void draw_field(
    int x,
    int y,
    int width,
    const char *label,
    const char *text,
    int active,
    int password_field
)
{
    int i;
    char masked[INPUT_MAX + 1];

    framebuffer_fill_rect(
        x,
        y,
        width,
        2,
        active ? COLOR_ACCENT : COLOR_BORDER
    );

    framebuffer_fill_rect(
        x,
        y + 2,
        width,
        44,
        COLOR_FIELD
    );

    font_draw_text(
        font,
        x,
        y - 12,
        label,
        COLOR_TEXT
    );

    if (password_field)
    {
        for (i = 0; text[i] != '\0'; i++)
            masked[i] = '*';

        masked[i] = '\0';

        font_draw_text(
            font,
            x + 16,
            y + 31,
            masked,
            COLOR_TEXT
        );
    }
    else
    {
        if (text[0] != '\0')
        {
            font_draw_text(
                font,
                x + 16,
                y + 31,
                text,
                COLOR_TEXT
            );
        }
        else
        {
            font_draw_text(
                font,
                x + 16,
                y + 31,
                "Username",
                COLOR_MUTED
            );
        }
    }
}

static void redraw_input_field(
    int field
)
{
    int width;
    int height;

    int panel_width;
    int panel_height;

    int panel_x;
    int panel_y;

    int field_x;
    int field_width;
    int field_y;

    const char *label;
    const char *text;
    int password_field;

    width = framebuffer_width();
    height = framebuffer_height();

    panel_width = 500;
    panel_height = 390;

    panel_x = (width - panel_width) / 2;
    panel_y = (height - panel_height) / 2;

    field_x = panel_x + 40;
    field_width = panel_width - 80;

    if (field == 0)
    {
        field_y = panel_y + 130;
        label = "Username";
        text = username;
        password_field = 0;
    }
    else
    {
        field_y = panel_y + 215;
        label = "Password";
        text = password;
        password_field = 1;
    }

    /*
     * Clear the complete field area.
     */
    framebuffer_fill_rect(
        field_x,
        field_y - 20,
        field_width,
        66,
        COLOR_PANEL
    );

    draw_field(
        field_x,
        field_y,
        field_width,
        label,
        text,
        active_field == field,
        password_field
    );
}

static void draw_login_screen(void)
{
    int width;
    int height;

    int panel_width;
    int panel_height;

    int panel_x;
    int panel_y;

    int field_x;
    int field_width;

    width = framebuffer_width();
    height = framebuffer_height();

    panel_width = 500;
    panel_height = 390;

    panel_x = (width - panel_width) / 2;
    panel_y = (height - panel_height) / 2;

    field_x = panel_x + 40;
    field_width = panel_width - 80;

    framebuffer_fill_rect(
        0,
        0,
        width,
        height,
        COLOR_BACKGROUND
    );

    framebuffer_fill_rect(
        panel_x,
        panel_y,
        panel_width,
        panel_height,
        COLOR_PANEL
    );

    framebuffer_fill_rect(
        panel_x,
        panel_y,
        panel_width,
        4,
        COLOR_ACCENT
    );

    font_draw_text(
        font,
        panel_x + 40,
        panel_y + 52,
        "tetoTerritory",
        COLOR_TEXT
    );

    font_draw_text(
        font,
        panel_x + 40,
        panel_y + 91,
        "Login",
        COLOR_ACCENT
    );

    draw_field(
        field_x,
        panel_y + 130,
        field_width,
        "Username",
        username,
        active_field == 0,
        0
    );

    draw_field(
        field_x,
        panel_y + 215,
        field_width,
        "Password",
        password,
        active_field == 1,
        1
    );

    framebuffer_fill_rect(
        panel_x + panel_width - 160,
        panel_y + 320,
        120,
        40,
        COLOR_ACCENT
    );

    font_draw_text(
        font,
        panel_x + panel_width - 132,
        panel_y + 348,
        "Login",
        COLOR_BACKGROUND
    );
}

static int keyboard_init(void)
{
    keyboard_fd = open(
        KEYBOARD_DEVICE,
        O_RDONLY | O_NONBLOCK
    );

    if (keyboard_fd < 0)
        return -1;

    return 0;
}

static void keyboard_shutdown(void)
{
    if (keyboard_fd >= 0)
    {
        close(keyboard_fd);
        keyboard_fd = -1;
    }
}

static int keyboard_update(void)
{
    struct input_event event;

    while (read(
        keyboard_fd,
        &event,
        sizeof(event)
    ) == sizeof(event))
    {
        if (event.type != EV_KEY)
            continue;

        if (event.code == KEY_LEFTSHIFT ||
            event.code == KEY_RIGHTSHIFT)
        {
            if (event.value == 1 ||
                event.value == 2)
            {
                shift_pressed = 1;
            }
            else if (event.value == 0)
            {
                shift_pressed = 0;
            }

            continue;
        }

        if (event.value != 1)
            continue;

        if (event.code == KEY_BACKSPACE)
        {
            input_backspace();

            redraw_input_field(
                active_field
            );

            continue;
        }

        if (event.code == KEY_TAB)
        {
            int old_field;

            old_field = active_field;

            active_field = !active_field;

            redraw_input_field(
                old_field
            );

            redraw_input_field(
                active_field
            );

            continue;
        }

        if (event.code == KEY_ENTER ||
            event.code == KEY_KPENTER)
        {
            if (login_check())
                return 1;

            continue;
        }

        {
            char character;

            character = key_to_char(
                event.code,
                shift_pressed
            );

            if (character != 0)
            {
                input_character(character);

                redraw_input_field(
                    active_field
                );
            }
        }
    }

    return 0;
}

int main(void)
{
    if (framebuffer_init() != 0)
        return 1;

    if (keyboard_init() != 0)
    {
        framebuffer_shutdown();
        return 1;
    }

    font = font_load(
        LOGIN_FONT_PATH,
        24
    );

    if (font == NULL)
    {
        keyboard_shutdown();
        framebuffer_shutdown();
        return 1;
    }

    username[0] = '\0';
    password[0] = '\0';

    draw_login_screen();

    while (1)
    {
        if (keyboard_update() == 1)
            break;
        usleep(16000);
    }

    font_destroy(font);
    font = NULL;

    keyboard_shutdown();
    framebuffer_shutdown();

    execl(
        TETODE_PATH,
        TETODE_PATH,
        (char *)NULL
    );

    perror("tetoLogin: failed to start /sbin/tetode");

    return 1;
}