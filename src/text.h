#ifndef TEXT_H
#define TEXT_H

#include "t3f/font.h"

#define TEXT_OBJECT_DISPLAY 0
#define TEXT_OBJECT_FLYER   1

typedef struct
{

	/* text data */
	char text[256];
	T3F_FONT * font;
	int val;

	/* positioning */
	int x, y, ex, ey, active;

	/* colors */
	bool shadow, outline;
	ALLEGRO_COLOR color;

	/* timing */
	int time;

	/* options */
	int type;

} TEXT_OBJECT;

typedef struct
{

	T3F_FONT * font;
	char glyph;
	ALLEGRO_COLOR color;
	float x, y, z;
	float vx, vy, vz;
	int active;

} TEXT_EFFECT_OBJECT;

void text_object_logic(TEXT_OBJECT * op, void * data);
void text_object_draw(ALLEGRO_BITMAP * bp, TEXT_OBJECT * op);
void text_effect_object_logic(TEXT_EFFECT_OBJECT * op);
void text_effect_object_draw(ALLEGRO_BITMAP * bp, TEXT_EFFECT_OBJECT * op);

#endif
