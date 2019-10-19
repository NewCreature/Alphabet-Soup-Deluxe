#include "t3f/t3f.h"
#include "instance.h"
#include "defines.h"
#include "types.h"
#include "dictionary.h"
#include "snake.h"
#include "effects.h"

void game_set_level(GAME * gp, DICTIONARY * dictionary)
{
	switch(gp->level)
	{
		case 0:
		{
			gp->word_min = 3 > dictionary[DICTIONARY_MAIN].word_max ? dictionary[DICTIONARY_MAIN].word_min : 3;
			gp->word_max = 5;
			gp->density = 10;
			break;
		}
		case 1:
		{
			gp->word_min = 3 > dictionary[DICTIONARY_MAIN].word_max ? dictionary[DICTIONARY_MAIN].word_min : 3;
			gp->word_max = 6;
			gp->density = 11;
			break;
		}
		case 2:
		{
			gp->word_min = 4 > dictionary[DICTIONARY_MAIN].word_max ? dictionary[DICTIONARY_MAIN].word_min : 4;
			gp->word_max = 7;
			gp->density = 12;
			break;
		}
		case 3:
		{
			gp->word_min = 4 > dictionary[DICTIONARY_MAIN].word_max ? dictionary[DICTIONARY_MAIN].word_min : 4;
			gp->word_max = 8;
			gp->density = 13;
			break;
		}
		case 4:
		{
			gp->word_min = 4 > dictionary[DICTIONARY_MAIN].word_max ? dictionary[DICTIONARY_MAIN].word_min : 4;
			gp->word_max = 9;
			gp->density = 14;
			break;
		}
		case 5:
		{
			gp->word_min = 4 > dictionary[DICTIONARY_MAIN].word_max ? dictionary[DICTIONARY_MAIN].word_min : 4;
			gp->word_max = 10;
			gp->density = 15;
			break;
		}
	}
}

void game_set_options(GAME * gp, DICTIONARY * dictionary)
{
	switch(gp->difficulty)
	{

		/* children */
		case 0:
		{
			gp->snake.delay = 65536;
			break;
		}

		/* easy */
		case 1:
		{
			gp->snake.delay = 20;
			break;
		}

		/* medium */
		case 2:
		{
			gp->snake.delay = 12;
			break;
		}

		/* hard */
		case 3:
		{
			gp->snake.delay = 8;
			break;
		}
	}
	gp->level = 0;
	game_set_level(gp, dictionary);
}

void game_set_bowl(GAME * gp)
{
	int i;

	/* generate map */
	map_clear(gp);
	switch(gp->mode)
	{
		case GAME_MODE_SCATTER_WORD:
		case GAME_MODE_BLIND:
		case GAME_MODE_ELIMINATION:
		case GAME_MODE_POISON:
		case GAME_MODE_FAT:
		case GAME_MODE_SKINNY:
		case GAME_MODE_COLD:
		case GAME_MODE_TEST:
		{
			for(i = 0; i < gp->density; i++)
			{
				map_add_letter(gp, gp->snake.word[gp->word_inc % strlen(gp->snake.word)]);
				gp->word_inc++;
			}
			map_auto_shuffle(gp, &gp->snake);
			break;
		}
		case GAME_MODE_NIBBLES:
		{
			map_add_letter(gp, gp->snake.word[gp->snake.word_pos]);
			break;
		}
	}
}

void game_add_letter(GAME * gp)
{
	switch(gp->mode)
	{
		case GAME_MODE_SCATTER_WORD:
		case GAME_MODE_BLIND:
		case GAME_MODE_ELIMINATION:
		case GAME_MODE_POISON:
		case GAME_MODE_FAT:
		case GAME_MODE_SKINNY:
		case GAME_MODE_COLD:
		case GAME_MODE_TEST:
		{
			map_add_letter(gp, gp->snake.word[gp->word_inc % strlen(gp->snake.word)]);
			gp->word_inc++;
			map_auto_shuffle(gp, &gp->snake);
			break;
		}
		case GAME_MODE_NIBBLES:
		{
			map_add_letter(gp, gp->snake.word[gp->snake.word_pos]);
			break;
		}
	}
}

void game_pick_word(GAME * gp, DICTIONARY * dictionary)
{
	switch(gp->mode)
	{
		case GAME_MODE_SCATTER_WORD:
		case GAME_MODE_NIBBLES:
		case GAME_MODE_BLIND:
		case GAME_MODE_POISON:
		case GAME_MODE_FAT:
		case GAME_MODE_SKINNY:
		case GAME_MODE_COLD:
		{
			gp->snake.word = dictionary_rand_word(&dictionary[DICTIONARY_MAIN], gp->word_min, gp->word_max);
			gp->snake.word_pos = 0;
			gp->word_inc = 0;
			break;
		}
		case GAME_MODE_ELIMINATION:
		case GAME_MODE_TEST:
		{
			gp->snake.word = dictionary_rand_word_take(&dictionary[DICTIONARY_MAIN], 0, 64);
			gp->snake.word_pos = 0;
			gp->word_inc = 0;
			break;
		}
	}
	gp->word_pos = 480;
}

void game_start(GAME * gp, DICTIONARY * dictionary, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	/* reload the dictionary */
	if(gp->mode == GAME_MODE_TEST)
	{
		dictionary_load(&dictionary[DICTIONARY_MAIN], "Spelling Test.txt", 0);
	}
	else
	{
		dictionary_load(&dictionary[DICTIONARY_MAIN], app->word_file[app->word_selected], 0);
	}

	game_set_options(gp, dictionary);

	/* set up player's snake */
	gp->snake.score = 0;
	gp->snake.words = 0;
	gp->snake.segment[0].letter = ' ';
	gp->snake.segment[0].x = 1;
	gp->snake.segment[0].y = 0;
	gp->snake.segment[0].px = 0;
	gp->snake.segment[0].py = 0;
	gp->snake.segment[0].nx = 2;
	gp->snake.segment[0].ny = 0;
	gp->snake.segment[1].letter = ' ';
	gp->snake.segment[1].x = 0;
	gp->snake.segment[1].y = 0;
	gp->snake.segment[1].px = 0;
	gp->snake.segment[1].py = 0;
	gp->snake.segment[1].nx = 1;
	gp->snake.segment[1].ny = 0;
	gp->snake.length = 2;
	gp->snake.left = gp->snake.delay;

//	gp->mode = mode;
	game_pick_word(gp, dictionary);
	game_set_bowl(gp);

	app->state = STATE_GAME;
}

int game_advance(GAME * gp, DICTIONARY * dictionary)
{
	int ol;
	int ret = 0;

	ol = gp->level;
	gp->level = gp->snake.words / 10;

	/* increase the density of the soup at level up */
	if(gp->level > ol)
	{
		switch(gp->mode)
		{
			case GAME_MODE_SCATTER_WORD:
			case GAME_MODE_BLIND:
			case GAME_MODE_POISON:
			case GAME_MODE_FAT:
			case GAME_MODE_SKINNY:
			case GAME_MODE_COLD:
			case GAME_MODE_ELIMINATION:
			case GAME_MODE_TEST:
			{
				map_add_letter(gp, gp->snake.word[rand() % gp->snake.word_pos]);
			}
			break;
		}
		ret = 1;
	}

	game_set_level(gp, dictionary);
	return ret;
}

void game_control(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(t3f_key[ALLEGRO_KEY_ESCAPE] && app->game_state == GAME_STATE_PLAY)
	{
		app->game_state = GAME_STATE_QUIT;
//		ncds_stop_music();
		t3f_clear_keys();
	}
}

void game_logic(GAME * gp, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int i, ok = 1;

	for(i = 0; i < MAX_TEXT; i++)
	{
		text_object_logic(&app->text[i], data);
	}
	for(i = 0; i < MAX_TEXT_EFFECT; i++)
	{
		text_effect_object_logic(&app->text_effect[i]);
	}
	switch(app->game_state)
	{
		case GAME_STATE_IN:
		{
			app->bowl_x += 8;
			if(app->bowl_x >= (640 - al_get_bitmap_width(app->image[IMAGE_GAME_BACKGROUND])) / 2)
			{
//				main_render(data);
//				main_draw(data);
				app->game_state = GAME_STATE_PLAY;
				t3f_play_music("data/music/bgm.xm");
			}
			break;
		}
		case GAME_STATE_DIE:
		case GAME_STATE_DIE2:
		{
			map_logic(gp);
			for(i = 0; i < MAX_TEXT; i++)
			{
				text_object_logic(&app->text[i], data);
				if(app->text[i].active)
				{
					ok = 0;
					break;
				}
			}
			if(ok)
			{
				app->game_state = GAME_STATE_OUT;
				t3f_stop_music();
//				key[KEY_ENTER] = 0;
			}
			break;
		}
		case GAME_STATE_PLAY:
		{
			if(gp->word_pos > 480 - 32)
			{
				gp->word_pos -= 2;
			}
			map_logic(gp);
			snake_move(&gp->snake, &gp->map, data);
			break;
		}
		case GAME_STATE_QUIT:
		{
			if(gp->word_pos > 480 - 32)
			{
				gp->word_pos -= 2;
			}
			map_logic(gp);
			if(t3f_key[ALLEGRO_KEY_N])
			{
				app->game_state = GAME_STATE_PLAY;
				t3f_clear_keys();
			}
			else if(t3f_key[ALLEGRO_KEY_Y])
			{
				app->game_state = GAME_STATE_DIE;
				t3f_clear_keys();
			}
			break;
		}
		case GAME_STATE_WIN:
		{
//			if(gp->word_pos > 480 - 32)
//			{
//				gp->word_pos -= 2;
//			}
			map_logic(gp);
			if(app->gtime > 0)
			{
				app->gtime--;
			}
			else
			{
				app->game_state = GAME_STATE_DIE2;
			}
			if(app->wy > 0 || app->gtime < 40)
			{
				app->wy -= 8;
			}
			break;
		}
		case GAME_STATE_OUT:
		{
			app->bowl_x -= 8;
			if(app->bowl_x < -al_get_bitmap_width(app->image[IMAGE_GAME_BACKGROUND]) - 8 - AS_BOWL_START_X)
			{
//				main_render();
//				main_draw();
				app->state = STATE_SCORES;
				app->scores_y = 480;
				app->score_place = hs_place(&app->high_scores[app->game.mode], app->game.snake.score);
				if(app->score_place >= 0)
				{
					app->score_pos = 0;
					app->high_scores[app->game.mode].lit = app->score_place;
					hs_insert(&app->high_scores[app->game.mode], app->score_place, app->game.snake.score, "");
//					high_scores[game.mode].place[score_place].score = game.snake.score;
//					high_scores[game.mode].place[score_place].name[0] = 0;
					app->score_state = SCORE_STATE_ENTER_IN;
				}
				else
				{
					app->high_scores[app->game.mode].lit = -1;
					app->score_state = SCORE_STATE_IN;
				}
				app->score_view = app->game.mode;
			}
			break;
		}
	}
}

/*void game_text_outline(char * text, T3F_FONT * fp, int x, int y, ALLEGRO_COLOR bg, ALLEGRO_COLOR fg)
{
	t3f_draw_text(fp, bg, x - 1, y, 0, 0, 0, 0, text);
	t3f_draw_text(fp, bg, x + 1, y, 0, 0, 0, 0, text);
	t3f_draw_text(fp, bg, x, y - 1, 0, 0, 0, 0, text);
	t3f_draw_text(fp, bg, x, y + 2, 0, 0, 0, 0, text);
	t3f_draw_text(fp, fg, x, y, 0, 0, 0, 0, text);
}

void game_text_outline_center(char * text, T3F_FONT * fp, int x, int y, ALLEGRO_COLOR bg, ALLEGRO_COLOR fg)
{
	t3f_draw_text(fp, bg, x - 1, y, 0, 0, 0, T3F_FONT_ALIGN_CENTER, text);
	t3f_draw_text(fp, bg, x + 1, y, 0, 0, 0, T3F_FONT_ALIGN_CENTER, text);
	t3f_draw_text(fp, bg, x, y - 1, 0, 0, 0, T3F_FONT_ALIGN_CENTER, text);
	t3f_draw_text(fp, bg, x, y + 2, 0, 0, 0, T3F_FONT_ALIGN_CENTER, text);
	t3f_draw_text(fp, fg, x, y, 0, 0, 0, T3F_FONT_ALIGN_CENTER, text);
} */

void game_draw_word(GAME * gp, void * data, T3F_FONT * fp, int x, int y)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	char text[256] = {0};
	int ofs, ofs2;

	strcpy(text, gp->snake.word);
	ofs = t3f_get_text_width(app->gfont[FONT_GAME_OUTLINE], text);
//	game_text_outline_center(text, gfont[FONT_GAME], x, y, makecol(0, 0, 0), makecol(128, 128, 192));

	/* draw shadow */
	t3f_draw_text(app->gfont[FONT_GAME_OUTLINE], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), x + 2 - ofs / 2, y + 2, 0, 0, 0, 0, text);

	/* show non-eaten letters */
	text[gp->snake.word_pos] = 0;
	ofs2 = t3f_get_text_width(app->gfont[FONT_GAME_OUTLINE], text);
	strcpy(text, &gp->snake.word[gp->snake.word_pos]);
	text[strlen(gp->snake.word) - gp->snake.word_pos] = 0;
	t3f_draw_text(app->gfont[FONT_GAME_OUTLINE], al_map_rgb(128, 128, 192), x + 2 - ofs / 2 + ofs2 - 2, y + 2, 0, 0, 0, 0, text);
//	game_text_outline(text, app->gfont[FONT_GAME], x + 2 - ofs / 2 + ofs2, y + 2, al_map_rgb(0, 0, 0), al_map_rgb(128, 128, 192));
	strcpy(text, gp->snake.word);

	/* show eaten letters */
	text[gp->snake.word_pos] = 0;
	t3f_draw_text(app->gfont[FONT_GAME_OUTLINE], al_map_rgb(192, 192, 255), x - ofs / 2, y, 0, 0, 0, 0, text);
//	game_text_outline(text, app->gfont[FONT_GAME], x - ofs / 2, y, al_map_rgb(0, 0, 0), al_map_rgb(192, 192, 255));
//	alfont_textout(buffer, gfont[FONT_GAME], text, x - ofs / 2, y, makecol(192, 192, 256));
}

void game_render(GAME * gp, void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	char score[16] = {0};
	int i;

	/* render the backdrop */
	t3f_select_view(NULL);
	al_draw_bitmap(app->image[IMAGE_MENU_BG], 0, 0, 0);
	t3f_select_view(app->letterbox_view);
	al_set_clipping_rectangle(0, 0, al_get_display_width(t3f_display), al_get_display_height(t3f_display));
	draw_shadow(app->image[IMAGE_GAME_BACKGROUND], app->bowl_x, (480 - al_get_bitmap_height(app->image[IMAGE_GAME_BACKGROUND])) / 2, 8, 8);
	al_draw_bitmap(app->image[IMAGE_GAME_BACKGROUND], app->bowl_x, (480 - al_get_bitmap_height(app->image[IMAGE_GAME_BACKGROUND])) / 2, 0);

	/* render the playing field */
	if(app->game_state == GAME_STATE_PLAY || app->game_state == GAME_STATE_DIE || app->game_state == GAME_STATE_DIE2 || app->game_state == GAME_STATE_IN || app->game_state == GAME_STATE_OUT || app->game_state == GAME_STATE_QUIT || app->game_state == GAME_STATE_WIN)
	{
		map_draw(NULL, gp, data, app->bowl_x + 16, app->bowl_y + 16);
		snake_draw(NULL, &gp->snake, data, app->bowl_x + 16, app->bowl_y + 16);
	}
	if(app->game_state == GAME_STATE_PLAY || app->game_state == GAME_STATE_DIE || app->game_state == GAME_STATE_DIE2 || app->game_state == GAME_STATE_QUIT || app->game_state == GAME_STATE_WIN)
	{
		if(app->game.mode != GAME_MODE_BLIND && app->game_state != GAME_STATE_WIN && app->game_state != GAME_STATE_DIE2)
		{
			game_draw_word(gp, data, app->gfont[FONT_GAME], 640 / 2, gp->word_pos);
		}
		sprintf(score, "%d", gp->snake.score);
		t3f_draw_text(app->gfont[FONT_GAME_OUTLINE], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 640 / 2 + 2, 2, 0, 0, 0, T3F_FONT_ALIGN_CENTER, score);
//		game_text_outline_center(score, app->gfont[FONT_GAME], 640 / 2 + 2, 2, al_map_rgb(0, 0, 0), al_map_rgb(0, 0, 0));
		t3f_draw_text(app->gfont[FONT_GAME_OUTLINE], al_map_rgb(255, 255, 255), 640 / 2, 0, 0, 0, 0, T3F_FONT_ALIGN_CENTER, score);
//		game_text_outline_center(score, app->gfont[FONT_GAME], 640 / 2, 0, al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
		for(i = 0; i < MAX_TEXT; i++)
		{
			text_object_draw(NULL, &app->text[i]);
		}
		for(i = 0; i < MAX_TEXT_EFFECT; i++)
		{
			text_effect_object_draw(NULL, &app->text_effect[i]);
		}
	}
	if(app->game_state == GAME_STATE_QUIT)
	{
		t3f_select_view(t3f_default_view);
		al_draw_filled_rectangle(0, 0, 960, 540, al_map_rgba(128, 128, 128, 128));
		t3f_select_view(app->letterbox_view);
		t3f_draw_text(app->gfont[FONT_GAME_BIG], t3f_color_white, 640 / 2, 216, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "Quit? (Y/N)");
//		game_text_outline_center("Quit? (Y/N)", app->gfont[FONT_GAME_BIG], 640 / 2, 216, al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
	}
	if(app->game_state == GAME_STATE_WIN)
	{
//		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
//		rectfill(buffer, 0, 0, buffer->w - 1, buffer->h - 1, makecol(128, 128, 128));
//		drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		if(gp->mode == GAME_MODE_TEST)
		{
			t3f_draw_text(app->gfont[FONT_GAME_BIG], t3f_color_white, 640 / 2, 216, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "Well Done!");
//			game_text_outline_center("Well Done!", app->gfont[FONT_GAME_BIG], 640 / 2, app->wy + 216, al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
//			game_text_outline_center("You Win!", gfont[FONT_GAME_BIG], SCREEN_W / 2, wy + 240, makecol(0, 0, 0), makecol(255, 255, 255));
		}
		else
		{
			t3f_draw_text(app->gfont[FONT_GAME_BIG], t3f_color_white, 640 / 2, app->wy + 192, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "Congratulations!");
//			game_text_outline_center("Congratulations!", app->gfont[FONT_GAME_BIG], 640 / 2, app->wy + 192, al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
			t3f_draw_text(app->gfont[FONT_GAME_BIG], t3f_color_white, 640 / 2, app->wy + 240, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "You Win!");
//			game_text_outline_center("You Win!", app->gfont[FONT_GAME_BIG], 640 / 2, app->wy + 240, al_map_rgb(0, 0, 0), al_map_rgb(255, 255, 255));
		}
	}
}
