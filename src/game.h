#ifndef GAME_H
#define GAME_H

#include "types.h"

#define AS_BOWL_START_X (t3f_default_view->width / 2 - app->letterbox_view->width / 2 - t3f_default_view->left)

void game_add_letter(GAME * gp);
void game_pick_word(GAME * gp, DICTIONARY * dictionary);
void game_start(GAME * gp, DICTIONARY * dictionary, void * data);
void game_control(void * data);
int game_advance(GAME * gp, DICTIONARY * dictionary);
void game_logic(GAME * gp, void * data);
void game_render(GAME * gp, void * data);
void game_draw(GAME * gp);

#endif
