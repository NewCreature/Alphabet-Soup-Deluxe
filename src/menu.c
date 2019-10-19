#include "t3f/t3f.h"
#include "instance.h"
#include "data.h"
#include "menu.h"
#include "effects.h"

char gmenu_dtext[256] = {0};
char gmenu_mtext[256] = {0};
char gmenu_wtext[256] = {0};
char gmenu_mvtext[256] = {0};
char gmenu_svtext[256] = {0};

void menu_proc_configure(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
	app->menu_state = MENU_STATE_CONFIG;
	config_init(data);
}

void menu_proc_scores(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_stop_music();
	t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
	app->menu_state = MENU_STATE_TO_SCORES;
}

void menu_proc_play(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	app->menu_state = MENU_STATE_TO_GAME;
	t3f_stop_music();
	t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
}

void menu_mode_text(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	strcpy(gmenu_mtext, score_title[app->game.mode]);
}

void menu_config_text(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	sprintf(gmenu_mvtext, "%d%%", app->music_vol);
	sprintf(gmenu_svtext, "%d%%", app->sound_vol);
}

void menu_proc_gamepad_left(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->game.snake.controller.gamepad >= 0)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->game.snake.controller.gamepad--;
		menu_config_text(data);
	}
}

void menu_proc_gamepad_right(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->game.snake.controller.gamepad < al_get_num_joysticks() - 1)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->game.snake.controller.gamepad++;
		menu_config_text(data);
	}
}

void menu_proc_mvol_left(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->music_vol > 0)
	{
		app->music_vol -= 5;
		t3f_set_music_volume(app->music_vol / 100.0);
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		menu_config_text(data);
	}
}

void menu_proc_mvol_right(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->music_vol < 100)
	{
		app->music_vol += 5;
		t3f_set_music_volume(app->music_vol / 100.0);
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		menu_config_text(data);
	}
}

void menu_proc_svol_left(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->sound_vol > 0)
	{
		app->sound_vol -= 5;
		t3f_set_sound_volume(app->sound_vol / 100.0);
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		menu_config_text(data);
	}
}

void menu_proc_svol_right(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->sound_vol < 100)
	{
		app->sound_vol += 5;
		t3f_set_sound_volume(app->sound_vol / 100.0);
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		menu_config_text(data);
	}
}

void menu_proc_mode_left(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->game.mode > 0)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->game.mode--;
		menu_mode_text(data);
	}
}

void menu_proc_mode_right(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->game.mode < game_modes - 1)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->game.mode++;
		menu_mode_text(data);
	}
}

void menu_difficulty_text(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	switch(app->game.difficulty)
	{
		case 0:
		{
			strcpy(gmenu_dtext, "Children");
			break;
		}
		case 1:
		{
			strcpy(gmenu_dtext, "Easy");
			break;
		}
		case 2:
		{
			strcpy(gmenu_dtext, "Medium");
			break;
		}
		case 3:
		{
			strcpy(gmenu_dtext, "Hard");
			break;
		}
	}
}

void menu_proc_difficulty_left(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->game.difficulty > 0)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->game.difficulty--;
		menu_difficulty_text(data);
	}
}

void menu_proc_difficulty_right(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->game.difficulty < 3)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->game.difficulty++;
		menu_difficulty_text(data);
	}
}

static char * get_filename(char * fn)
{
	int pos = strlen(fn) - 1;

	while(pos >= 0)
	{
		if(fn[pos] == '/' || fn[pos] == '\\')
		{
			return &fn[pos + 1];
		}
		pos--;
	}
	return fn;
}

void menu_dictionary_text(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	strcpy(gmenu_wtext, get_filename(app->word_file[app->word_selected]));
}

void menu_proc_dictionary_left(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->word_selected > 0)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->word_selected--;
		dictionary_load(&app->dictionary[DICTIONARY_MAIN], app->word_file[app->word_selected], 0);
		menu_dictionary_text(data);
	}
}

void menu_proc_dictionary_right(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->word_selected < app->word_files - 1)
	{
		t3f_play_sample(app->sound[SOUND_MENU_OPTION], 0.5, 0.0, 1.0);
		app->word_selected++;
		dictionary_load(&app->dictionary[DICTIONARY_MAIN], app->word_file[app->word_selected], 0);
		menu_dictionary_text(data);
	}
}

void menu_proc_dictionary_enter(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
//	char fn[1024] = {0};
//	char ofn[1024] = {0};

	t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
/*	if(file_select_ex("Import Dictionary", fn, "txt", 1024, 320, 240))
	{
		strcpy(ofn, fn);
		replace_extension(ofn, ofn, "asd", 1024);
		strcpy(ofn, get_filename(ofn));
		dictionary_load(&dictionary[DICTIONARY_MAIN], fn, 1);
		dictionary_save(&dictionary[DICTIONARY_MAIN], ofn);
		build_dictionary_list();
	} */
}

void config_init(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	memset(&app->menu, sizeof(GMENU), 0);

	/* set up menu */
	memset(&app->menu, sizeof(GMENU), 0);
	app->menu.item[0].type = MENU_ITEM_TYPE_TEXT;
	app->menu.item[0].data = "Configure";
	app->menu.item[0].ox = 0;
	app->menu.item[0].oy = 12;
	app->menu.item[0].font = app->gfont[FONT_MENU_HEADER];

	app->menu.item[1].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[1].ox = 3;
	app->menu.item[1].oy = 36;

	app->menu.item[2].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[2].data = "Music Volume";
	app->menu.item[2].ox = 0;
	app->menu.item[2].oy = 42;
	app->menu.item[2].left_proc = menu_proc_mvol_left;
	app->menu.item[2].right_proc = menu_proc_mvol_right;
	app->menu.item[2].font = app->gfont[FONT_MENU];

	app->menu.item[3].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[3].ox = 1;
	app->menu.item[3].oy = 59;

	app->menu.item[4].type = MENU_ITEM_TYPE_TEXT;
	app->menu.item[4].data = gmenu_mvtext;
	app->menu.item[4].ox = 0;
	app->menu.item[4].oy = 62;
	app->menu.item[4].font = app->gfont[FONT_MENU];

	app->menu.item[5].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[5].ox = 2;
	app->menu.item[5].oy = 79;

	app->menu.item[6].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[6].data = "Sound Volume";
	app->menu.item[6].ox = 0;
	app->menu.item[6].oy = 83;
	app->menu.item[6].left_proc = menu_proc_svol_left;
	app->menu.item[6].right_proc = menu_proc_svol_right;
	app->menu.item[6].font = app->gfont[FONT_MENU];

	app->menu.item[7].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[7].ox = 1;
	app->menu.item[7].oy = 100;

	app->menu.item[8].type = MENU_ITEM_TYPE_TEXT;
	app->menu.item[8].data = gmenu_svtext;
	app->menu.item[8].ox = 0;
	app->menu.item[8].oy = 104;
	app->menu.item[8].font = app->gfont[FONT_MENU];

	menu_config_text(data);

	app->menu.items = 9;
	app->menu.selected = 2;
}

void menu_init(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	/* set up menu */
	memset(&app->menu, sizeof(GMENU), 0);
	app->menu.item[0].type = MENU_ITEM_TYPE_TEXT;
	app->menu.item[0].data = "Options";
	app->menu.item[0].ox = 0;
	app->menu.item[0].oy = 12;
	app->menu.item[0].font = app->gfont[FONT_MENU_HEADER];

	app->menu.item[1].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[1].ox = 3;
	app->menu.item[1].oy = 36;

	app->menu.item[2].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[2].data = "Game Mode";
	app->menu.item[2].help = "Select the mode of play.";
	app->menu.item[2].ox = 0;
	app->menu.item[2].oy = 42;
	app->menu.item[2].left_proc = menu_proc_mode_left;
	app->menu.item[2].right_proc = menu_proc_mode_right;
	app->menu.item[2].font = app->gfont[FONT_MENU];

	app->menu.item[3].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[3].ox = 1;
	app->menu.item[3].oy = 59;

	app->menu.item[4].type = MENU_ITEM_TYPE_TEXT;
	app->menu.item[4].data = gmenu_mtext;
	app->menu.item[4].ox = 0;
	app->menu.item[4].oy = 62;
	app->menu.item[4].font = app->gfont[FONT_MENU];
	menu_mode_text(data);

	app->menu.item[5].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[5].ox = 2;
	app->menu.item[5].oy = 79;

	app->menu.item[6].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[6].data = "Difficulty";
	app->menu.item[6].help = "Set the difficulty level.";
	app->menu.item[6].ox = 0;
	app->menu.item[6].oy = 83;
	app->menu.item[6].left_proc = menu_proc_difficulty_left;
	app->menu.item[6].right_proc = menu_proc_difficulty_right;
	app->menu.item[6].font = app->gfont[FONT_MENU];

	app->menu.item[7].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[7].ox = 1;
	app->menu.item[7].oy = 100;

	app->menu.item[8].type = MENU_ITEM_TYPE_TEXT;
	app->menu.item[8].data = gmenu_dtext;
	app->menu.item[8].ox = 0;
	app->menu.item[8].oy = 103;
	app->menu.item[8].font = app->gfont[FONT_MENU];
	menu_difficulty_text(data);

	app->menu.item[9].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[9].ox = 2;
	app->menu.item[9].oy = 120;

	app->menu.item[10].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[10].data = "Dictionary";
	app->menu.item[10].help = "Select a dictionary for the game to use.";
	app->menu.item[10].ox = 0;
	app->menu.item[10].oy = 124;
	app->menu.item[10].enter_proc = menu_proc_dictionary_enter;
	app->menu.item[10].left_proc = menu_proc_dictionary_left;
	app->menu.item[10].right_proc = menu_proc_dictionary_right;
	app->menu.item[10].font = app->gfont[FONT_MENU];

	app->menu.item[11].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[11].ox = 1;
	app->menu.item[11].oy = 141;

	app->menu.item[12].type = MENU_ITEM_TYPE_TEXT;
	app->menu.item[12].data = gmenu_wtext;
	app->menu.item[12].ox = 0;
	app->menu.item[12].oy = 144;
	app->menu.item[12].font = app->gfont[FONT_MENU];
	menu_dictionary_text(data);

	app->menu.item[13].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[13].ox = 2;
	app->menu.item[13].oy = 161;

	app->menu.item[14].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[14].data = "Configure";
	app->menu.item[14].help = "Edit the game configuration.";
	app->menu.item[14].ox = 0;
	app->menu.item[14].oy = 165;
	app->menu.item[14].enter_proc = menu_proc_configure;
	app->menu.item[14].font = app->gfont[FONT_MENU];

	app->menu.item[15].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[15].ox = 2;
	app->menu.item[15].oy = 182;

	app->menu.item[16].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[16].data = "View Scores";
	app->menu.item[16].help = "See the best players.";
	app->menu.item[16].ox = 0;
	app->menu.item[16].oy = 185;
	app->menu.item[16].enter_proc = menu_proc_scores;
	app->menu.item[16].font = app->gfont[FONT_MENU];

	app->menu.item[17].type = MENU_ITEM_TYPE_BAR;
	app->menu.item[17].ox = 2;
	app->menu.item[17].oy = 202;

	app->menu.item[18].type = MENU_ITEM_TYPE_CHOICE;
	app->menu.item[18].data = "Play";
	app->menu.item[18].help = "Start the game.";
	app->menu.item[18].ox = 0;
	app->menu.item[18].oy = 205;
	app->menu.item[18].font = app->gfont[FONT_MENU];
	app->menu.item[18].enter_proc = menu_proc_play;
	app->menu.selected = 18;

	app->menu.items = 19;
}

void gmenu_next_item(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	app->menu.selected++;
	if(app->menu.selected >= app->menu.items)
	{
		app->menu.selected = 0;
	}
}

void gmenu_prev_item(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	app->menu.selected--;
	if(app->menu.selected < 0)
	{
		app->menu.selected = app->menu.items - 1;
	}
}

void gmenu_next(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	gmenu_next_item(data);
	while(app->menu.item[app->menu.selected].type != MENU_ITEM_TYPE_CHOICE)
	{
		gmenu_next_item(data);
	}
}

void gmenu_prev(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	gmenu_prev_item(data);
	while(app->menu.item[app->menu.selected].type != MENU_ITEM_TYPE_CHOICE)
	{
		gmenu_prev_item(data);
	}
}

void menu_logic(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
//	int f = frame;
	switch(app->menu_state)
	{
		case MENU_STATE_TO_MENU:
		{
			app->can_y += 8;
			if(app->can_y >= CAN_START_Y)
			{
				app->menu_state = MENU_STATE_MAIN;
			}
			break;
		}
		case MENU_STATE_TO_OS:
		{
			app->can_y -= 8;
			if(app->can_y < CAN_END_Y)
			{
				app->menu_state = MENU_STATE_CREDITS;
				t3f_clear_keys();
//				state = STATE_FIN;
			}
			break;
		}
		case MENU_STATE_MAIN:
		{
			if(t3f_key[ALLEGRO_KEY_ESCAPE])
			{
				app->menu_state = MENU_STATE_TO_OS;
				t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_ENTER])
			{
				app->menu_state = MENU_STATE_TO_OPTIONS;
				t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_ENTER] = 0;
			}
			break;
		}
		case MENU_STATE_TO_OPTIONS:
		{
			app->can_ofs += 8;
			if(app->can_ofs - CAN_START_OFS >= al_get_bitmap_width(app->image[IMAGE_SOUP_CAN]) / 2)
			{
				app->menu_state = MENU_STATE_OPTIONS;
			}
			break;
		}
		case MENU_STATE_OPTIONS:
		{
			if(t3f_key[ALLEGRO_KEY_ESCAPE])
			{
				app->menu_state = MENU_STATE_TO_MAIN;
				t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
			}

			/* menu input */
			if(t3f_key[ALLEGRO_KEY_UP])
			{
				gmenu_prev(data);
				t3f_play_sample(app->sound[SOUND_MENU_MOVE], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_UP] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_DOWN])
			{
				gmenu_next(data);
				t3f_play_sample(app->sound[SOUND_MENU_MOVE], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_DOWN] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_LEFT] && app->menu.item[app->menu.selected].left_proc)
			{
				app->menu.item[app->menu.selected].left_proc();
				t3f_key[ALLEGRO_KEY_LEFT] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_RIGHT] && app->menu.item[app->menu.selected].right_proc)
			{
				app->menu.item[app->menu.selected].right_proc();
				t3f_key[ALLEGRO_KEY_RIGHT] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_ENTER] && app->menu.item[app->menu.selected].enter_proc)
			{
				app->menu.item[app->menu.selected].enter_proc();
				t3f_key[ALLEGRO_KEY_ENTER] = 0;
			}
			break;
		}
		case MENU_STATE_CONFIG:
		{
			if(t3f_key[ALLEGRO_KEY_ESCAPE])
			{
				app->menu_state = MENU_STATE_OPTIONS;
				t3f_play_sample(app->sound[SOUND_MENU_ENTER], 0.5, 0.0, 1.0);
				menu_init(data);
				t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
			}

			/* menu input */
			if(t3f_key[ALLEGRO_KEY_UP])
			{
				gmenu_prev(data);
				t3f_play_sample(app->sound[SOUND_MENU_MOVE], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_UP] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_DOWN])
			{
				gmenu_next(data);
				t3f_play_sample(app->sound[SOUND_MENU_MOVE], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_DOWN] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_LEFT] && app->menu.item[app->menu.selected].left_proc)
			{
				app->menu.item[app->menu.selected].left_proc();
				t3f_key[ALLEGRO_KEY_LEFT] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_RIGHT] && app->menu.item[app->menu.selected].right_proc)
			{
				app->menu.item[app->menu.selected].right_proc();
				t3f_key[ALLEGRO_KEY_RIGHT] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_ENTER] && app->menu.item[app->menu.selected].enter_proc)
			{
				app->menu.item[app->menu.selected].enter_proc();
				t3f_key[ALLEGRO_KEY_ENTER] = 0;
			}
			break;
		}
		case MENU_STATE_TO_MAIN:
		{
			app->can_ofs -= 8;
			if(app->can_ofs - CAN_START_OFS <= 0)
			{
				app->menu_state = MENU_STATE_MAIN;
			}
			break;
		}
		case MENU_STATE_TO_GAME:
		{
			app->can_y += 8;
			if(t3f_project_y(app->can_y, app->can->z + app->can->width / 2) > 480)
			{
				game_start(&app->game, app->dictionary, data);
				app->bowl_x = -al_get_bitmap_width(app->image[IMAGE_GAME_BACKGROUND]) - 8 - AS_BOWL_START_X;
				app->bowl_y = 32;
				app->game_state = GAME_STATE_IN;
			}
			break;
		}
		case MENU_STATE_TO_SCORES:
		{
			app->can_y -= 8;
			if(app->can_y < CAN_END_Y)
			{
				app->state = STATE_SCORES;
				app->score_state = SCORE_STATE_IN;
				app->scores_x = 0;
				app->scores_y = 480;
				app->high_scores[app->score_view].lit = -1;
			}
			break;
		}
		case MENU_STATE_FROM_SCORES:
		{
			app->can_y += 8;
			if(app->can_y >= CAN_START_Y)
			{
				app->menu_state = MENU_STATE_OPTIONS;
			}
			break;
		}
		case MENU_STATE_CREDITS:
		{
			if(t3f_key_pressed() || app->credits.y + app->credits.item[app->credits.items - 1].oy + 32 < 0)
			{
				t3f_clear_keys();
				t3f_exit();
			}
			credits_logic(&app->credits);
		}
	}
	as_move_3d_can(app->can, app->can_x, app->can_y, 200, app->can_ofs);
}

void menu_render_can(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	ALLEGRO_TRANSFORM identity;
	ALLEGRO_STATE old_state;
	int i;

	/* clear the surface */
	al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM | ALLEGRO_STATE_TARGET_BITMAP);
	al_identity_transform(&identity);
	al_set_target_bitmap(app->image[IMAGE_MENU_SURFACE]);
	al_use_transform(&identity);
	al_clear_to_color(app->menu_bg_color);
	al_draw_rectangle(50.0 + 0.5, 10.0 + 0.5, al_get_bitmap_width(app->image[IMAGE_MENU_SURFACE]) - 50.0 - 1.0 + 0.5, (app->menu.item[app->menu.items - 1].oy + 19.0) - 1.0 + 0.5, al_map_rgb(0, 0, 0), 1.0);
	al_draw_rectangle(51.0 + 0.5, 10.0 + 0.5, al_get_bitmap_width(app->image[IMAGE_MENU_SURFACE]) - 51.0 - 1.0 + 0.5, (app->menu.item[app->menu.items - 1].oy + 19.0) - 1.0 + 0.5, al_map_rgb(0, 0, 0), 1.0);

	for(i = 0; i < app->menu.items; i++)
	{
		switch(app->menu.item[i].type)
		{
			case MENU_ITEM_TYPE_CHOICE:
			{
				if(i == app->menu.selected)
				{
					t3f_draw_textf(app->menu.item[i].font, al_map_rgb(255, 0, 0),  al_get_bitmap_width(app->image[IMAGE_MENU_SURFACE]) / 2, app->menu.item[i].oy - 2, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "%s", app->menu.item[i].data);
				}
				else
				{
					t3f_draw_textf(app->menu.item[i].font, al_map_rgb(0, 0, 0),  al_get_bitmap_width(app->image[IMAGE_MENU_SURFACE]) / 2, app->menu.item[i].oy - 2, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "%s", app->menu.item[i].data);
				}
				break;
			}
			case MENU_ITEM_TYPE_TEXT:
			{
				t3f_draw_textf(app->menu.item[i].font, al_map_rgb(0, 0, 0),  al_get_bitmap_width(app->image[IMAGE_MENU_SURFACE]) / 2, app->menu.item[i].oy - 2, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "%s", app->menu.item[i].data);
				break;
			}
			case MENU_ITEM_TYPE_BAR:
			{
				al_draw_filled_rectangle(56, app->menu.item[i].oy, al_get_bitmap_width(app->image[IMAGE_MENU_SURFACE]) - 56.0 - 1.0, app->menu.item[i].oy + app->menu.item[i].ox, al_map_rgb(0, 0, 0));
				break;
			}
		}
	}
	al_restore_state(&old_state);
}

void menu_render(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_select_view(NULL);
	al_draw_bitmap(app->image[IMAGE_MENU_BG], 0, 0, 0);
	t3f_select_view(app->letterbox_view);
	menu_render_can(data);
	as_render_3d_can(app->can);
	if(app->menu_state == MENU_STATE_OPTIONS)
	{
		t3f_draw_textf(app->gfont[FONT_MENU_OUTLINE], al_map_rgba_f(0.0, 0.0, 0.0, 0.5), 640 / 2 + 2, 456 - 16 + 2, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "%s", app->menu.item[app->menu.selected].help);
		t3f_draw_textf(app->gfont[FONT_MENU_OUTLINE], al_map_rgb(255, 255, 255), 640 / 2, 456 - 16, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "%s", app->menu.item[app->menu.selected].help);
	}

	if(app->menu_state == MENU_STATE_CREDITS)
	{
		credits_render(NULL, &app->credits);
	}
}

//int misc, lw_pl, lw_pos;

void scores_logic(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int k;

	switch(app->score_state)
	{
		case SCORE_STATE_VIEW:
		{
			if(t3f_key[ALLEGRO_KEY_ESCAPE])
			{
				app->score_state = SCORE_STATE_OUT;
				t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_LEFT] && app->score_view > 0)
			{
				app->score_view--;
				app->high_scores[app->score_view].lit = -1;
				t3f_play_sample(app->sound[SOUND_PAPER], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_LEFT] = 0;
			}
			if(t3f_key[ALLEGRO_KEY_RIGHT] && app->score_view < game_modes - 1)
			{
				app->score_view++;
				app->high_scores[app->score_view].lit = -1;
				t3f_play_sample(app->sound[SOUND_PAPER], 0.5, 0.0, 1.0);
				t3f_key[ALLEGRO_KEY_RIGHT] = 0;
			}
			break;
		}
		case SCORE_STATE_ENTER:
		{
			if(t3f_key_pressed())
			{
				k = t3f_read_key(0);
				switch(k)
				{
					/* enter */
					case 0xD:
					{
		            	app->high_scores[app->game.mode].place[app->score_place].name[app->score_pos] = '\0';
						app->high_scores[app->game.mode].lit = -1;
						app->score_state = SCORE_STATE_VIEW;
						app->game.snake.score = 0;
						t3f_key[ALLEGRO_KEY_ENTER] = 0;
						break;
					}

					/* escape */
					case 0x1B:
					{
						strcpy(app->high_scores[app->game.mode].place[app->score_place].name, "No Name");
						app->high_scores[app->game.mode].lit = -1;
						app->score_state = SCORE_STATE_VIEW;
						app->game.snake.score = 0;
						t3f_key[ALLEGRO_KEY_ESCAPE] = 0;
						break;
					}

					/* backspace */
					case 0x8:
					{
            			app->high_scores[app->game.mode].place[app->score_place].name[app->score_pos] = '\0';
       					if(app->score_pos > 0)
       					{
	               			app->score_pos--;
           					app->high_scores[app->game.mode].place[app->score_place].name[app->score_pos] = '\0';
       					}
						break;
					}

					/* normal key */
					default:
					{
            			app->high_scores[app->game.mode].place[app->score_place].name[app->score_pos] = k;
       					app->score_pos++;
       					app->high_scores[app->game.mode].place[app->score_place].name[app->score_pos] = '\0';
						break;
					}
				}
			}
			break;
		}
		case SCORE_STATE_IN:
		{
			app->scores_y -= 8;
			if(app->scores_y <= 0)
			{
				app->score_state = SCORE_STATE_VIEW;
				t3f_clear_keys();
			}
			break;
		}
		case SCORE_STATE_ENTER_IN:
		{
			app->scores_y -= 8;
			if(app->scores_y <= 0)
			{
				t3f_clear_keys();
				app->score_state = SCORE_STATE_ENTER;
			}
			break;
		}
		case SCORE_STATE_OUT:
		{
			app->scores_y += 8;
			if(app->scores_y > 480)
			{
				app->can_x = 640 / 2;
				app->can_y = CAN_END_Y;
//				app->can_ofs = CAN_START_OFS;
				app->state = STATE_MENU;
				app->menu_state = MENU_STATE_FROM_SCORES;
				t3f_play_music("data/music/title.xm");
			}
			break;
		}
	}
}

void scores_render(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_select_view(NULL);
	al_draw_bitmap(app->image[IMAGE_MENU_BG], 0, 0, 0);
	t3f_select_view(app->letterbox_view);
	draw_shadow(app->image[IMAGE_PAPER], 0, app->scores_y, 8, 8);
	al_draw_bitmap(app->image[IMAGE_PAPER], 0, app->scores_y, 0);
	t3f_draw_textf(app->gfont[FONT_GAME], al_map_rgb(0, 0, 0), 640 / 2, app->scores_y + 55, 0, 0, 0, T3F_FONT_ALIGN_CENTER, "%s", score_title[app->score_view]);
//	hs_render_ex(buffer, &high_scores[score_view], app->gfont[FONT_GAME], scores_x, scores_y, 160, 32, 31, 24, makecol(0, 0, 0));
	hs_render_ex(NULL, &app->high_scores[app->score_view], app->gfont[FONT_GAME], app->scores_x, app->scores_y + 22 - 5, 160, 32, al_map_rgb(0, 0, 0), al_map_rgb(192, 192, 192), al_map_rgba_f(0.0, 0.0, 0.0, 0.5));
}
