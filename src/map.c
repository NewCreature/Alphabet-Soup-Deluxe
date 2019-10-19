#include "t3f/t3f.h"
#include "instance.h"
#include "defines.h"
#include "types.h"

void map_auto_shuffle(GAME * gp, SNAKE * sp)
{
	int i, p;

	for(i = 0; i < MAX_LETTERS; i++)
	{
		if(gp->map.letter[i].active)
		{
			if(gp->map.letter[i].val == sp->word[sp->word_pos])
			{
				return;
			}
		}
	}

	/* if we get this far, shuffle */
	p = sp->word_pos;
	for(i = 0; i < MAX_LETTERS; i++)
	{
		if(gp->map.letter[i].active)
		{
			gp->map.letter[i].val = sp->word[p];
			p++;
			if(p >= strlen(sp->word))
			{
				p = 0;
			}
		}
	}
}

void map_update_blocked(GAME * gp, SNAKE * sp)
{
	int i, j;

	/* clear the map */
	for(i = 0; i < MAP_H; i++)
	{
		for(j = 0; j < MAP_W; j++)
		{
			gp->map.blocked[i][j] = 0;
		}
	}

	/* block off active letters */
	for(i = 0; i < MAX_LETTERS; i++)
	{
		if(gp->map.letter[i].active)
		{
			gp->map.blocked[gp->map.letter[i].y][gp->map.letter[i].x] = 1;
		}
	}

	/* block off snake segments */
	for(i = 0; i < sp->length; i++)
	{
		gp->map.blocked[sp->segment[i].y][sp->segment[i].x] = 1;
	}

	/* block off spaces immediately next to snake head */
	if(sp->segment[0].y > 0)
	{
		if(sp->segment[0].x > 0)
		{
			gp->map.blocked[sp->segment[0].y - 1][sp->segment[0].x - 1] = 1;
		}
		gp->map.blocked[sp->segment[0].y - 1][sp->segment[0].x] = 1;
		if(sp->segment[0].x < MAP_W - 1)
		{
			gp->map.blocked[sp->segment[0].y - 1][sp->segment[0].x + 1] = 1;
		}
	}
	if(sp->segment[0].x > 0)
	{
		gp->map.blocked[sp->segment[0].y][sp->segment[0].x - 1] = 1;
	}
	gp->map.blocked[sp->segment[0].y][sp->segment[0].x] = 1;
	if(sp->segment[0].x < MAP_W - 1)
	{
		gp->map.blocked[sp->segment[0].y][sp->segment[0].x + 1] = 1;
	}
	if(sp->segment[0].y < MAP_H - 1)
	{
		if(sp->segment[0].x > 0)
		{
			gp->map.blocked[sp->segment[0].y + 1][sp->segment[0].x - 1] = 1;
		}
		gp->map.blocked[sp->segment[0].y + 1][sp->segment[0].x] = 1;
		if(sp->segment[0].x < MAP_W - 1)
		{
			gp->map.blocked[sp->segment[0].y + 1][sp->segment[0].x + 1] = 1;
		}
	}
}

void map_letter_time(GAME * gp, int i)
{
	switch(gp->level)
	{
		case 0:
		{
			gp->map.letter[i].time = 120 + (rand() % 5) * 60;
			break;
		}
		case 1:
		{
			gp->map.letter[i].time = 120 + (rand() % 4) * 60;
			break;
		}
		case 2:
		{
			gp->map.letter[i].time = 60 + (rand() % 4) * 60;
			break;
		}
	}
}

void map_new_letter(GAME * gp, int val, int x, int y)
{
	int i;

	for(i = 0; i < MAX_LETTERS; i++)
	{
		if(!gp->map.letter[i].active)
		{
			gp->map.letter[i].val = val;
			gp->map.letter[i].x = x;
			gp->map.letter[i].y = y;
			gp->map.letter[i].active = 1;
			gp->map.letter[i].f = 0;
			map_letter_time(gp, i);
			break;
		}
	}
}

void map_add_letter(GAME * gp, char letter)
{
	int loc;
	int i, j;
	int smap[MAP_H][MAP_W] = {{0}};

	/* find a place to put the letter */
	map_update_blocked(gp, &gp->snake);
	gp->map.list_size = 0;
	for(i = 0; i < gp->snake.length; i++)
	{
		smap[gp->snake.segment[i].y][gp->snake.segment[i].x] = 1;
	}
	for(i = 0; i < MAP_H; i++)
	{
		for(j = 0; j < MAP_W; j++)
		{
			if(!gp->map.blocked[i][j] && !smap[i][j])
			{
				gp->map.list[gp->map.list_size] = i * MAP_W + j;
				gp->map.list_size++;
			}
		}
	}

	/* generate the new letter */
	loc = rand() % gp->map.list_size;
	map_new_letter(gp, letter, gp->map.list[loc] % MAP_W, gp->map.list[loc] / MAP_W);
}

void map_re_letter(GAME * gp, char * wp)
{
	int i;

	for(i = 0; i < MAX_LETTERS; i++)
	{
		if(gp->map.letter[i].active)
		{
			gp->map.letter[i].val = wp[gp->word_inc % strlen(wp)];
			gp->word_inc++;
		}
	}
}

void map_clear(GAME * gp)
{
	int i;

	for(i = 0; i < MAX_LETTERS; i++)
	{
		gp->map.letter[i].active = 0;
	}
}

void map_logic(GAME * gp)
{
	int i;
	int x, y;
	int mx[4] = {0};
	int my[4] = {0};
	int moves = 0;
	int r;

	for(i = 0; i < MAX_LETTERS; i++)
	{

		/* animate */
		if(gp->map.letter[i].active)
		{
			gp->map.letter[i].f++;
		}

		/* see if letter is ready to move */
		if(gp->map.letter[i].time > 0 && gp->mode != GAME_MODE_COLD)
		{
			gp->map.letter[i].time--;
			if(gp->map.letter[i].time <= 0)
			{

				/* update solid map */
				map_update_blocked(gp, &gp->snake);
				x = gp->map.letter[i].x;
				y = gp->map.letter[i].y;
				moves = 0;

				/* is moving up possible? */
				if(y > 0)
				{
					if(!gp->map.blocked[y - 1][x])
					{
						mx[moves] = 0;
						my[moves] = -1;
						moves++;
					}
				}

				/* is moving down possible? */
				if(y < MAP_H - 1)
				{
					if(!gp->map.blocked[y + 1][x])
					{
						mx[moves] = 0;
						my[moves] = 1;
						moves++;
					}
				}

				/* is moving right possible? */
				if(x < MAP_W - 1)
				{
					if(!gp->map.blocked[y][x + 1])
					{
						mx[moves] = 1;
						my[moves] = 0;
						moves++;
					}
				}

				/* is moving left possible? */
				if(x > 0)
				{
					if(!gp->map.blocked[y][x - 1])
					{
						mx[moves] = -1;
						my[moves] = 0;
						moves++;
					}
				}
				if(moves)
				{
					r = rand() % moves;
					gp->map.letter[i].x += mx[r];
					gp->map.letter[i].y += my[r];
				}
				gp->map.letter[i].time = 120 + (rand() % 5) * 60;
			}
		}
	}
}

void map_draw(ALLEGRO_BITMAP * bp, GAME * gp, void * data, int ox, int oy)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int i;
	char str[2] = {'\0', '\0'};

	for(i = 0; i < MAX_LETTERS; i++)
	{
		if(gp->map.letter[i].active)
		{
//			draw_sprite(bp, bg[1], gp->map.letter[i].x * 32, gp->map.letter[i].y * 32);
			str[0] = gp->map.letter[i].val;
			t3f_draw_text(app->gfont[FONT_GAME], al_map_rgb(200, 200, 200), ox + gp->map.letter[i].x * 32 + 5, oy + gp->map.letter[i].y * 32 + 4 - 4, 0, 0, 0, 0, str);
			draw_ani(bp, app->animation[ANIMATION_APPEAR_FG], ox + gp->map.letter[i].x * 32, oy + gp->map.letter[i].y * 32, gp->map.letter[i].f);
		}
	}
}
