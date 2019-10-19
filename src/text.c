#include "t3f/t3f.h"
#include "t3f/font.h"
#include "includes.h"
#include "instance.h"

void text_object_logic(TEXT_OBJECT * op, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(op->active)
	{
		switch(op->type)
		{
			case TEXT_OBJECT_DISPLAY:
			{
				break;
			}
			case TEXT_OBJECT_FLYER:
			{
				if(op->time > 0)
				{
					op->time--;
				}
				else
				{
					if(op->ex < op->x)
					{
						op->x -= 2;
					}
					else if(op->ex > op->x)
					{
						op->x += 2;
					}
					if(op->ey < op->y)
					{
						op->y -= 2;
					}
					else if(op->ey > op->y)
					{
						op->y += 2;
					}
					else if(op->x == op->ex && op->y == op->ey)
					{
						op->active = 0;
						app->game.snake.score += op->val;
					}
				}
				break;
			}
		}
	}
}

void text_object_draw(ALLEGRO_BITMAP * bp, TEXT_OBJECT * op)
{
	if(op->active)
	{
		switch(op->type)
		{
			case TEXT_OBJECT_DISPLAY:
			case TEXT_OBJECT_FLYER:
			{
/*				if(op->shadow)
				{
					if(op->outline)
					{
						t3f_draw_text(op->font, al_map_rgb(0, 0, 0), op->x - 1 + 2, op->y + 2, 0, 0, 0, 0, op->text);
						t3f_draw_text(op->font, al_map_rgb(0, 0, 0), op->x + 1 + 2, op->y + 2, 0, 0, 0, 0, op->text);
						alfont_textout_centre(bp, op->font, op->text, op->x + 2, op->y - 1 + 2, makecol(0, 0, 0));
						alfont_textout_centre(bp, op->font, op->text, op->x + 2, op->y + 1 + 2, makecol(0, 0, 0));
					}
					else
					{
						alfont_textout_centre(bp, op->font, op->text, op->x + 2, op->y + 2, makecol(0, 0, 0));
					}
				}
				else
				{
					if(op->outline)
					{
						alfont_textout_centre(bp, op->font, op->text, op->x - 1, op->y, op->outline);
						alfont_textout_centre(bp, op->font, op->text, op->x + 1, op->y, op->outline);
						alfont_textout_centre(bp, op->font, op->text, op->x, op->y - 1, op->outline);
						alfont_textout_centre(bp, op->font, op->text, op->x, op->y + 1, op->outline);
					}
				} */
				t3f_draw_text(op->font, op->color, op->x, op->y, 0, 0, 0, T3F_FONT_ALIGN_CENTER, op->text);
				break;
			}
		}
	}
}

void text_effect_object_logic(TEXT_EFFECT_OBJECT * op)
{
	if(op->active)
	{
		op->x += op->vx;
		op->y += op->vy;
		if(op->y >= 480)
		{
			op->active = 0;
		}
		op->z += op->vz;
		op->vy += 0.5;
	}
}

void text_effect_object_draw(ALLEGRO_BITMAP * bp, TEXT_EFFECT_OBJECT * op)
{
	if(op->active && op->font)
	{
		t3f_draw_textf(op->font, t3f_color_black, op->x - 1, op->y, op->z, 0, 0, 0, "%c", op->glyph);
		t3f_draw_textf(op->font, t3f_color_black, op->x + 1, op->y, op->z, 0, 0, 0, "%c", op->glyph);
		t3f_draw_textf(op->font, t3f_color_black, op->x, op->y - 1, op->z, 0, 0, 0, "%c", op->glyph);
		t3f_draw_textf(op->font, t3f_color_black, op->x, op->y + 1, op->z, 0, 0, 0, "%c", op->glyph);
		t3f_draw_textf(op->font, op->color, op->x, op->y, op->z, 0, 0, 0, "%c", op->glyph);
	}
}
