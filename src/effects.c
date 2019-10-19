#include "t3f/t3f.h"
#include "effects.h"
#include "animate.h"

void draw_shadow(ALLEGRO_BITMAP * bp, int x, int y, int ox, int oy)
{
	al_draw_tinted_bitmap(bp, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), x + ox, y + oy, 0);
}
