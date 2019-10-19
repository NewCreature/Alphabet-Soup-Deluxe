#ifndef SNAKE_H
#define SNAKE_H

#include "map.h"
#include "input.h"

void snake_compile(SNAKE * sp);
void snake_move(SNAKE * sp, MAP * mp, void * data);
void snake_draw(ALLEGRO_BITMAP * bp, SNAKE * sp, void * data, int ox, int oy);

#endif
