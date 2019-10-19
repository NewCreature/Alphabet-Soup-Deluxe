#include "t3f/t3f.h"
#include "t3f/font.h"
#include "credits.h"

void credits_add_item(CREDITS * cp, char * text, T3F_FONT * fp, int oy, ALLEGRO_COLOR color)
{
	strcpy(cp->item[cp->items].text, text);
	cp->item[cp->items].font = fp;
	cp->item[cp->items].oy = oy;
	cp->item[cp->items].color = color;
	cp->items++;
}

void credits_logic(CREDITS * cp)
{
	cp->y -= cp->vy;
}

void credits_render(ALLEGRO_BITMAP * bp, CREDITS * cp)
{
	int i;

	for(i = 0; i < cp->items; i++)
	{
		t3f_draw_text(cp->item[i].font, al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 640 / 2 + 2, cp->y + cp->item[i].oy + 2, 0.0, 0.0, 0.0, T3F_FONT_ALIGN_CENTER, cp->item[i].text);
		t3f_draw_text(cp->item[i].font, cp->item[i].color, 640 / 2, cp->y + cp->item[i].oy, 0.0, 0.0, 0.0, T3F_FONT_ALIGN_CENTER, cp->item[i].text);
	}
}
