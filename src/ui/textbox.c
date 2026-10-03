#include <ui/textbox.h>

#include <stdlib.h>
#include <string.h>

teto_textbox_t *textbox_create(
    int x,
    int y,
    int width,
    int height
)
{
    teto_textbox_t *textbox = malloc(sizeof(teto_textbox_t));

    if (textbox == NULL)
        return NULL;

    textbox->x = x;
    textbox->y = y;
    textbox->width = width;
    textbox->height = height;
    textbox->text = NULL;
    textbox->focused = 0;

    return textbox;
}

void textbox_destroy(teto_textbox_t *textbox)
{
    if (textbox == NULL)
        return;

    free(textbox->text);
    free(textbox);
}

void textbox_set_text(
    teto_textbox_t *textbox,
    const char *text
)
{
    char *new_text;

    if (textbox == NULL)
        return;

    if (text == NULL)
    {
        free(textbox->text);
        textbox->text = NULL;
        return;
    }

    new_text = malloc(strlen(text) + 1);

    if (new_text == NULL)
        return;

    strcpy(new_text, text);

    free(textbox->text);
    textbox->text = new_text;
}

const char *textbox_get_text(
    const teto_textbox_t *textbox
)
{
    if (textbox == NULL)
        return NULL;

    return textbox->text;
}

void textbox_set_focus(
    teto_textbox_t *textbox,
    int focused
)
{
    if (textbox == NULL)
        return;

    textbox->focused = focused;
}

void textbox_draw(
    teto_textbox_t *textbox
)
{
    if (textbox == NULL)
        return;

    /*
     * Rendering will be implemented when the
     * tetoTerritory UI renderer is added.
     */
}