#ifndef TETOTERRITORY_MOUSE_H
#define TETOTERRITORY_MOUSE_H

int mouse_init(void);
void mouse_shutdown(void);

int mouse_x(void);
int mouse_y(void);

int mouse_left(void);
int mouse_middle(void);
int mouse_right(void);

void mouse_update(void);
void mouse_draw(void);

#endif