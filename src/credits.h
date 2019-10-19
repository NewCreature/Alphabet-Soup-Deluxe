#ifndef CREDITS_H
#define CREDITS_H

#include "t3f/font.h"

#define MAX_CREDITS 256

typedef struct
{

	char text[256];
	T3F_FONT * font;
	int oy;
	ALLEGRO_COLOR color;

} CREDIT_ITEM;

typedef struct
{

	CREDIT_ITEM item[MAX_CREDITS];
	int y, vy;
	int items;

} CREDITS;

void credits_add_item(CREDITS * cp, char * text, T3F_FONT * fp, int oy, ALLEGRO_COLOR color);
void credits_logic(CREDITS * cp);
void credits_render(ALLEGRO_BITMAP * bp, CREDITS * cp);

#endif
