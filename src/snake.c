#include "t3f/t3f.h"
#include "t3f/font.h"
#include "includes.h"
#include "instance.h"
#include "game.h"

void snake_delete(SNAKE * sp, int seg)
{
	int i;

	for(i = seg; i < sp->length - 1; i++)
	{
		sp->segment[i].letter = sp->segment[i + 1].letter;
	}
	sp->length--;
}

void snake_insert(SNAKE * sp, char val)
{
	int i;

	for(i = sp->length; i > 0; i--)
	{
		sp->segment[i].letter = sp->segment[i - 1].letter;
	}
	sp->segment[0].letter = val;
	if(sp->length < SNAKE_MAX_SEGMENTS - 1)
	{
		sp->length++;
	}
}

void snake_add(SNAKE * sp, char val)
{
	sp->segment[sp->length].letter = val;
	if(sp->length < SNAKE_MAX_SEGMENTS - 1)
	{
		sp->length++;
	}
}

void snake_control(SNAKE * sp)
{
	read_controller(&sp->controller);
	if(sp->segment[0].nx != sp->segment[0].x)
	{
		if(sp->controller.up == 1)
		{
			sp->segment[0].ny--;
			sp->segment[0].nx = sp->segment[0].x;
			sp->left = 1;
			t3f_key[sp->controller.up_key] = 0;
		}
		else if(sp->controller.down == 1)
		{
			sp->segment[0].ny++;
			sp->segment[0].nx = sp->segment[0].x;
			sp->left = 1;
			t3f_key[sp->controller.down_key] = 0;
		}
		else if(sp->segment[0].nx > sp->segment[0].x && sp->controller.right)
		{
//			sp->segment[0].nx++;
//			sp->segment[0].nx = sp->segment[0].x;
			sp->left = 1;
			t3f_key[sp->controller.right_key] = 0;
		}
		else if(sp->segment[0].nx < sp->segment[0].x && sp->controller.left)
		{
//			sp->segment[0].nx++;
//			sp->segment[0].nx = sp->segment[0].x;
			sp->left = 1;
			t3f_key[sp->controller.left_key] = 0;
		}
	}
	else if(sp->segment[0].ny != sp->segment[0].y)
	{
		if(sp->controller.left == 1)
		{
			sp->segment[0].nx--;
			sp->segment[0].ny = sp->segment[0].y;
			sp->left = 1;
			t3f_key[sp->controller.left_key] = 0;
		}
		else if(sp->controller.right == 1)
		{
			sp->segment[0].nx++;
			sp->segment[0].ny = sp->segment[0].y;
			sp->left = 1;
			t3f_key[sp->controller.right_key] = 0;
		}
		else if(sp->segment[0].ny > sp->segment[0].y && sp->controller.down)
		{
//			sp->segment[0].nx++;
//			sp->segment[0].nx = sp->segment[0].x;
			sp->left = 1;
			t3f_key[sp->controller.down_key] = 0;
		}
		else if(sp->segment[0].ny < sp->segment[0].y && sp->controller.up)
		{
//			sp->segment[0].nx++;
//			sp->segment[0].nx = sp->segment[0].x;
			sp->left = 1;
			t3f_key[sp->controller.up_key] = 0;
		}
	}
//	if(sp->controller.fire)
//	{
//		map_re_letter(&game.map, sp->word);
//	}
/*	if(sp->controller.rotate)
	{
		j = sp->segment[sp->length - 1].letter;
		for(i = sp->length - 1; i > 0; i--)
		{
			sp->segment[i].letter = sp->segment[i - 1].letter;
		}
		sp->segment[0].letter = j;
	} */
}

/* see if we run into ourself */
int snake_crunch(SNAKE * sp)
{
	int j;

	for(j = 0; j < sp->length - 1; j++)
	{
		if(j != 0 && sp->segment[0].x == sp->segment[j].x && sp->segment[0].y == sp->segment[j].y)
		{
			return 1;
		}
	}
	return 0;
}

void snake_move(SNAKE * sp, MAP * mp, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int i, j;
	int points = 5;
	char c[2] = {0};
	int cx;

	snake_control(sp);

	/* see if it's time for snake to move */
	sp->left--;
	if(sp->left <= 0)
	{

		/* move the head */
		sp->segment[0].px = sp->segment[0].x;
		sp->segment[0].py = sp->segment[0].y;
		sp->segment[0].x = sp->segment[0].nx;
		sp->segment[0].y = sp->segment[0].ny;
		sp->segment[0].nx += sp->segment[0].x - sp->segment[0].px;
		sp->segment[0].ny += sp->segment[0].y - sp->segment[0].py;
		sp->segment[1].nx = sp->segment[0].px;
		sp->segment[1].ny = sp->segment[0].py;

		/* player dies */
		if(sp->segment[0].x < 0 || sp->segment[0].x >= MAP_W || sp->segment[0].y < 0 || sp->segment[0].y >= MAP_H || snake_crunch(sp))
		{
			sp->segment[0].x = sp->segment[0].px;
			sp->segment[0].y = sp->segment[0].py;
			app->game_state = GAME_STATE_DIE;
			return;
		}

		/* move the body */
		for(i = 1; i < sp->length; i++)
		{
			sp->segment[i].px = sp->segment[i].x;
			sp->segment[i].py = sp->segment[i].y;
			sp->segment[i].x = sp->segment[i].nx;
			sp->segment[i].y = sp->segment[i].ny;
			sp->segment[i].nx += sp->segment[i].x - sp->segment[i].px;
			sp->segment[i].ny += sp->segment[i].y - sp->segment[i].py;
			sp->segment[i + 1].nx = sp->segment[i].px;
			sp->segment[i + 1].ny = sp->segment[i].py;
		}

		/* player collects letter from playing field */
		for(i = 0; i < MAX_LETTERS; i++)
		{
			if(mp->letter[i].active)
			{
				if(mp->letter[i].x == sp->segment[0].x && mp->letter[i].y == sp->segment[0].y)
				{
					sp->segment[sp->length].x = sp->segment[sp->length - 1].px;
					sp->segment[sp->length].y = sp->segment[sp->length - 1].py;
					sp->segment[sp->length].nx = sp->segment[sp->length - 1].x;
					sp->segment[sp->length].ny = sp->segment[sp->length - 1].y;

					/* player collects correct letter */
					if(mp->letter[i].val == sp->word[sp->word_pos])
					{
						snake_add(sp, mp->letter[i].val);
						sp->word_pos++;

						/* player finishes word */
						if(sp->word_pos >= strlen(sp->word))
						{

							/* calculate starting point of text effect objects */
							cx = 640 / 2 - t3f_get_text_width(app->gfont[FONT_GAME], sp->word) / 2;

							/* create exploding text objects */
							for(j = 0; j < strlen(sp->word); j++)
							{
								c[0] = sp->word[j];
								app->text_effect[j].glyph = sp->word[j];
								app->text_effect[j].x = cx - 1;
								app->text_effect[j].y = 480 - 32 - 1;
//								text_effect[j].y = 0;
								app->text_effect[j].z = 0;
								app->text_effect[j].vx = 0;
//								text_effect[j].vx = fdiv(itofix(rand() % 7 - 4), itofix(3));
								app->text_effect[j].vy = -10 - rand() % 8 / 4.0;
								app->text_effect[j].vz = -30;
								app->text_effect[j].active = 1;
								app->text_effect[j].font = app->gfont[FONT_GAME];
								app->text_effect[j].color = al_map_rgb(192, 192, 255);
								cx += t3f_get_text_width(app->gfont[FONT_GAME], c);
							}

							if(app->game.mode != GAME_MODE_FAT)
							{
								sp->length -= strlen(sp->word);
							}
							for(j = 0; j < strlen(sp->word); j++)
							{
								points *= 2;
							}
//							sp->score += points;
							for(j = 0; j < MAX_TEXT; j++)
							{
								if(!app->text[j].active)
								{
									sprintf(app->text[j].text, "+%d", points);
									app->text[j].val = points;
									app->text[j].font = app->gfont[FONT_GAME];
									app->text[j].x = 640 / 2;
									app->text[j].y = 32;
									app->text[j].ex = 640 / 2;
									app->text[j].ey = 0;
									app->text[j].time = 60;
									app->text[j].color = al_map_rgb(255, 255, 255);
									app->text[j].shadow = 0;
									app->text[j].outline = 0;
									app->text[j].type = TEXT_OBJECT_FLYER;
									app->text[j].active = 1;
									break;
								}
							}
							sp->words++;
							if(app->dictionary[DICTIONARY_MAIN].words > 0)
							{
								if(game_advance(&app->game, app->dictionary))
								{
									t3f_play_sample(app->sound[SOUND_LEVEL], 0.5, 0.0, 1.0);
								}
								else
								{
									t3f_play_sample(app->sound[SOUND_WORD], 0.5, 0.0, 1.0);
								}
								game_pick_word(&app->game, app->dictionary);
								map_re_letter(&app->game, sp->word);
								if(app->game.mode != GAME_MODE_FAT && app->game.mode != GAME_MODE_SKINNY)
								{
									snake_insert(sp, ' ');
								}
								app->game.word_pos = 480;
							}
							else
							{
								app->game_state = GAME_STATE_WIN;
								app->gtime = 300;
								app->wy = 400;
								t3f_stop_music();
								t3f_play_sample(app->sound[SOUND_CLAP], 0.5, 0.0, 1.0);
							}
						}
					}
					else
					{
						if(app->game.mode == GAME_MODE_POISON)
						{
							app->game_state = GAME_STATE_DIE;
						}
						snake_insert(sp, ' ');
					}
					mp->letter[i].active = 0;

					/* delete the letter and add new letter to playing field */
					t3f_play_sample(app->sound[SOUND_BITE], 0.5, 0.0, 1.0);
					game_add_letter(&app->game);
					break;
				}
			}

		}
		sp->left = sp->delay;
	}
}

void snake_draw(ALLEGRO_BITMAP * bp, SNAKE * sp, void * data, int ox, int oy)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int i;
	ALLEGRO_BITMAP * sbp;

	if(sp->segment[1].x > sp->segment[0].x)
	{
		sbp = app->image[IMAGE_HEAD_LEFT];
	}
	if(sp->segment[1].x < sp->segment[0].x)
	{
		sbp = app->image[IMAGE_HEAD_RIGHT];
	}
	if(sp->segment[1].y > sp->segment[0].y)
	{
		sbp = app->image[IMAGE_HEAD_UP];
	}
	if(sp->segment[1].y < sp->segment[0].y)
	{
		sbp = app->image[IMAGE_HEAD_DOWN];
	}
	al_draw_bitmap(sbp, ox + sp->segment[0].x * 32, oy + sp->segment[0].y * 32, 0);
	for(i = 1; i < sp->length; i++)
	{
		if(i + 1 < sp->length)
		{
			if(sp->segment[i].x < sp->segment[i - 1].x && sp->segment[i].y < sp->segment[i + 1].y)
			{
				sbp = app->image[IMAGE_BODY_DUR];
			}
			else if(sp->segment[i].x < sp->segment[i - 1].x && sp->segment[i].y > sp->segment[i + 1].y)
			{
				sbp = app->image[IMAGE_BODY_DDR];
			}
			else if(sp->segment[i].x > sp->segment[i - 1].x && sp->segment[i].y < sp->segment[i + 1].y)
			{
				sbp = app->image[IMAGE_BODY_DUL];
			}
			else if(sp->segment[i].x > sp->segment[i - 1].x && sp->segment[i].y > sp->segment[i + 1].y)
			{
				sbp = app->image[IMAGE_BODY_DDL];
			}
			else if(sp->segment[i].x < sp->segment[i + 1].x && sp->segment[i].y < sp->segment[i - 1].y)
			{
				sbp = app->image[IMAGE_BODY_DUR];
			}
			else if(sp->segment[i].x < sp->segment[i + 1].x && sp->segment[i].y > sp->segment[i - 1].y)
			{
				sbp = app->image[IMAGE_BODY_DDR];
			}
			else if(sp->segment[i].x > sp->segment[i + 1].x && sp->segment[i].y < sp->segment[i - 1].y)
			{
				sbp = app->image[IMAGE_BODY_DUL];
			}
			else if(sp->segment[i].x > sp->segment[i + 1].x && sp->segment[i].y > sp->segment[i - 1].y)
			{
				sbp = app->image[IMAGE_BODY_DDL];
			}
			else
			{
				if(sp->segment[i].nx == sp->segment[i].x)
				{
					sbp = app->image[IMAGE_BODY_V];
				}
				else
				{
					sbp = app->image[IMAGE_BODY_H];
				}
			}
		}
		else
		{
			if(sp->segment[i].x > sp->segment[i - 1].x)
			{
				sbp = app->image[IMAGE_TAIL_LEFT];
			}
			else if(sp->segment[i].x < sp->segment[i - 1].x)
			{
				sbp = app->image[IMAGE_TAIL_RIGHT];
			}
			else if(sp->segment[i].y > sp->segment[i - 1].y)
			{
				sbp = app->image[IMAGE_TAIL_UP];
			}
			else
			{
				sbp = app->image[IMAGE_TAIL_DOWN];
			}
		}
		al_draw_bitmap(sbp, ox + sp->segment[i].x * 32, oy + sp->segment[i].y * 32, 0);
	}
}
