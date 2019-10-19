#ifndef _MAP_H
#define _MAP_H

#include "types.h"

//void map_auto_shuffle(GAME * gp, SNAKE * sp);
void map_new_letter(GAME * gp, int val, int x, int y);
void map_add_letter(GAME * gp, char letter);
void map_re_letter(GAME * gp, char * wp);
void map_clear(GAME * gp);
void map_auto_shuffle(GAME * gp, SNAKE * sp);
void map_logic(GAME * gp);
void map_draw(ALLEGRO_BITMAP * bp, GAME * gp, void * data, int ox, int oy);

#endif
