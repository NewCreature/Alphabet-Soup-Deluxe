#include "t3f/t3f.h"
#include "instance.h"
#include "hs.h"
#include "animate.h"
#include "data.h"
#include "map.h"
#include "dictionary.h"
#include "game.h"
#include "effects.h"
#include "menu.h"
#include "text.h"
#include "credits.h"

const char * score_title[MAX_GAME_MODES] = {"Scatter Word", "Blind Worm", "Elimination", "Poison Letter", "Fat Worm", "Skinny Worm", "Cold Soup", "Nibbles", "Spelling Test"};
int game_modes = 9;

void word_file_add(void * data, const char * fn, int attrib, int param)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	strcpy(app->word_file[app->word_files], fn);
	app->word_files++;
}

void build_dictionary_list(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	app->word_files = 0;
	word_file_add(data, "data/dictionaries/default.asd", 0, 0);
	word_file_add(data, "data/dictionaries/kids.asd", 0, 0);
	word_file_add(data, "data/dictionaries/strange.asd", 0, 0);
//	for_each_file("*.asd", FA_ARCH | FA_RDONLY, word_file_add, 0);
	dictionary_load(&app->dictionary[DICTIONARY_MAIN], app->word_file[app->word_selected], 0);
}

void load_mem_file(void * data, char * fn)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	ALLEGRO_FILE * fp;
	int i;

	fp = al_fopen(fn, "rb");
	if(fp)
	{
		app->mem_buffer_size = al_fsize(fp);
		app->mem_buffer = malloc(app->mem_buffer_size);
		if(app->mem_buffer)
		{
			for(i = 0; i < app->mem_buffer_size; i++)
			{
				app->mem_buffer[i] = al_fgetc(fp);
			}
		}
	}
}

void free_mem_file(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	if(app->mem_buffer)
	{
		free(app->mem_buffer);
		app->mem_buffer = NULL;
	}
}

int copy_file(char * f1, char * f2)
{
	ALLEGRO_FILE * fp1, * fp2;

	fp1 = al_fopen(f1, "rb");
	if(!fp1)
	{
		return 0;
	}
	fp2 = al_fopen(f2, "wb");
	if(!fp2)
	{
		al_fclose(fp1);
		return 0;
	}
	while(!al_feof(fp1))
	{
		al_fputc(fp2, al_fgetc(fp1));
	}
	al_fclose(fp1);
	al_fclose(fp2);
	return 1;
}

static void save_config_value(const char * name, int value)
{
	char buf[64];

	sprintf(buf, "%d", value);
	al_set_config_value(t3f_config, "Game Config", name, buf);
}

static int load_config_value(const char * name, int df)
{
	const char * val;

	val = al_get_config_value(t3f_config, "Game Config", name);
	if(val)
	{
		return atoi(val);
	}
	return df;
}

int config_save(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	save_config_value("Music Volume", app->music_vol);
	save_config_value("Sound Volume", app->sound_vol);
	save_config_value("Last Game Mode", app->game.mode);
	save_config_value("Last Game Difficulty", app->game.difficulty);
	save_config_value("Dictionary", app->word_selected);

	return 1;
}

int config_load(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	app->music_vol = load_config_value("Music Volume", 100);
	t3f_set_music_volume(app->music_vol / 100.0);
	app->sound_vol = load_config_value("Sound Volume", 100);
	t3f_set_sound_volume(app->sound_vol / 100.0);
	app->game.mode = load_config_value("Last Game Mode", 0);
	app->game.difficulty = load_config_value("Last Game Difficulty", 2);
	app->word_selected = load_config_value("Dictionary", 0);

	return 1;
}

bool load_wav(ALLEGRO_SAMPLE ** sp, const char * fn)
{
	*sp = al_load_sample(fn);
	if(*sp)
	{
		return true;
	}
	printf("Error loading %s\n", fn);
	return false;
}

bool load_pcx(ALLEGRO_BITMAP ** bp, const char * fn, PALETTE * pal, bool trans)
{
	*bp = al_load_bitmap(fn);
	if(*bp)
	{
		if(trans)
		{
			al_convert_mask_to_alpha(*bp, al_map_rgb(255, 0, 255));
		}
		return true;
	}
	printf("Error loading %s\n", fn);
	return false;
}

static void grey_to_shade(ALLEGRO_BITMAP * bp)
{
	ALLEGRO_STATE old_state;
	ALLEGRO_COLOR pixel;
	unsigned char r, g, b, l;
	int i, j;

	al_store_state(&old_state, ALLEGRO_STATE_TARGET_BITMAP);
	al_set_target_bitmap(bp);
	al_lock_bitmap(bp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
	for(i = 0; i < al_get_bitmap_height(bp); i++)
	{
		for(j = 0; j < al_get_bitmap_width(bp); j++)
		{
			pixel = al_get_pixel(bp, j, i);
			al_unmap_rgb(pixel, &r, &g, &b);
			l = ((int)r + (int)g + (int)b) / 3;
			al_put_pixel(j, i, al_map_rgba(0, 0, 0, 255 - l));
		}
	}
	al_unlock_bitmap(bp);
	al_restore_state(&old_state);
}

ALLEGRO_COLOR makecol(int r, int g, int b)
{
	return al_map_rgba(r, g, b, 255);
}

bool data_load(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	ALLEGRO_STATE old_state;
	PALETTE palette;
	int i;

	/* load sounds */
	if(!load_wav(&app->sound[SOUND_BITE], "data/sounds/eat.wav"))
	{
		return false;
	}
	if(!load_wav(&app->sound[SOUND_MENU_MOVE], "data/sounds/navigate.wav"))
	{
		return false;
	}
	if(!load_wav(&app->sound[SOUND_MENU_ENTER], "data/sounds/select.wav"))
	{
		return false;
	}
	if(!load_wav(&app->sound[SOUND_MENU_OPTION], "data/sounds/option.wav"))
	{
		return false;
	}
	if(!load_wav(&app->sound[SOUND_WORD], "data/sounds/word.wav"))
	{
		return false;
	}
	if(!load_wav(&app->sound[SOUND_LEVEL], "data/sounds/level.wav"))
	{
		return false;
	}
	if(!load_wav(&app->sound[SOUND_PAPER], "data/sounds/paper.wav"))
	{
		return false;
	}
	if(!load_wav(&app->sound[SOUND_CLAP], "data/sounds/clap.wav"))
	{
		return false;
	}

	/* load images */
	if(!load_pcx(&app->image[IMAGE_GAME_BACKGROUND], "data/images/bowl.pcx", NULL, false))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_MENU_BG], "data/images/background.pcx", NULL, false))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_SOUP_CAN], "data/images/can_texture.pcx", NULL, false))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_SOUP_SHADOW], "data/images/can_shadow.pcx", NULL, false))
	{
		return false;
	}
	grey_to_shade(app->image[IMAGE_SOUP_SHADOW]);
	app->image[IMAGE_MENU_SURFACE] = al_create_sub_bitmap(app->image[IMAGE_SOUP_CAN], 0, 90, al_get_bitmap_width(app->image[IMAGE_SOUP_CAN]) / 2, al_get_bitmap_height(app->image[IMAGE_SOUP_CAN]) - 90 - 4);
	if(!app->image[IMAGE_MENU_SURFACE])
	{
		return false;
	}
	app->menu_bg_color = al_get_pixel(app->image[IMAGE_MENU_SURFACE], 0, 0);
	al_store_state(&old_state, ALLEGRO_STATE_NEW_BITMAP_PARAMETERS);
	al_set_new_bitmap_flags(al_get_new_bitmap_flags() | ALLEGRO_NO_PRESERVE_TEXTURE);
	app->image[IMAGE_CAN_BUFFER] = al_create_bitmap(CAN_R * 2, CAN_H);
	if(!app->image[IMAGE_CAN_BUFFER])
	{
		al_restore_state(&old_state);
		return false;
	}
	al_restore_state(&old_state);
	if(!load_pcx(&app->image[IMAGE_PAPER], "data/images/score_sheet.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_BODY_H], "data/images/snake/body_horizontal.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_BODY_V], "data/images/snake/body_vertical.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_BODY_DUL], "data/images/snake/body_up_left.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_BODY_DUR], "data/images/snake/body_up_right.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_BODY_DDL], "data/images/snake/body_down_left.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_BODY_DDR], "data/images/snake/body_down_right.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_HEAD_UP], "data/images/snake/head_up.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_HEAD_DOWN], "data/images/snake/head_down.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_HEAD_LEFT], "data/images/snake/head_left.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_HEAD_RIGHT], "data/images/snake/head_right.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_TAIL_UP], "data/images/snake/tail_up.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_TAIL_DOWN], "data/images/snake/tail_down.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_TAIL_LEFT], "data/images/snake/tail_left.pcx", NULL, true))
	{
		return false;
	}
	if(!load_pcx(&app->image[IMAGE_TAIL_RIGHT], "data/images/snake/tail_right.pcx", NULL, true))
	{
		return false;
	}

	app->game.snake.controller.left_key = ALLEGRO_KEY_LEFT;
	app->game.snake.controller.right_key = ALLEGRO_KEY_RIGHT;
	app->game.snake.controller.up_key = ALLEGRO_KEY_UP;
	app->game.snake.controller.down_key = ALLEGRO_KEY_DOWN;
	app->game.snake.controller.fire_key = ALLEGRO_KEY_ENTER;
	app->game.snake.controller.rotate_key = ALLEGRO_KEY_SPACE;
	build_dictionary_list(app);

	/* load fonts */
	app->gfont[FONT_GAME] = t3f_generate_font("data/fonts/game.ttf", 32, 0);
	app->gfont[FONT_GAME_OUTLINE] = t3f_generate_font("data/fonts/game.ttf", 32, T3F_FONT_OUTLINE);
	app->gfont[FONT_GAME_BIG] = t3f_generate_font("data/fonts/game.ttf", 48, T3F_FONT_OUTLINE);
	app->gfont[FONT_MENU_HEADER] = t3f_generate_font("data/fonts/menu.ttf", 24, 0);
	app->gfont[FONT_MENU] = t3f_generate_font("data/fonts/menu.ttf", 16, 0);
	app->gfont[FONT_MENU_OUTLINE] = t3f_generate_font("data/fonts/menu.ttf", 24, T3F_FONT_OUTLINE);

	/* load animations */
	app->animation[ANIMATION_APPEAR_FG] = load_ani("data/animations/letter_appear.ani", &palette);
	if(!app->animation[ANIMATION_APPEAR_FG])
	{
		printf("Error loading animation!\n");
		return false;
	}

	app->can = as_create_3d_can(app->image[IMAGE_SOUP_CAN], NULL, NULL, al_get_bitmap_width(app->image[IMAGE_SOUP_CAN]) / 2, al_get_bitmap_height(app->image[IMAGE_SOUP_CAN]) + 64 + 30);
	if(!app->can)
	{
		printf("Error creating 3D can!\n");
		return false;
	}

	/* create text effect objects */
	for(i = 0; i < MAX_TEXT_EFFECT; i++)
	{
		app->text_effect[i].active = 0;
	}

	/* create credits list */
	app->credits.items = 0;
	app->credits.y = 480;
	app->credits.vy = 1;

	credits_add_item(&app->credits, "Game Concept", app->gfont[FONT_GAME_OUTLINE], 0, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Todd Cope", app->gfont[FONT_GAME_OUTLINE], 32, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Game Design", app->gfont[FONT_GAME_OUTLINE], 128, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Todd Cope", app->gfont[FONT_GAME_OUTLINE], 160, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Brandy Cope", app->gfont[FONT_GAME_OUTLINE], 192, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Music", app->gfont[FONT_GAME_OUTLINE], 288, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Todd Cope", app->gfont[FONT_GAME_OUTLINE], 320, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Brandy Cope", app->gfont[FONT_GAME_OUTLINE], 352, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Sound", app->gfont[FONT_GAME_OUTLINE], 448, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Todd Cope", app->gfont[FONT_GAME_OUTLINE], 480, makecol(255, 255, 255));
	credits_add_item(&app->credits, "www.partnersinrhyme.com", app->gfont[FONT_GAME_OUTLINE], 512, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Graphics", app->gfont[FONT_GAME_OUTLINE], 608, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Todd Cope", app->gfont[FONT_GAME_OUTLINE], 640, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Web Design", app->gfont[FONT_GAME_OUTLINE], 736, makecol(192, 192, 255));
	credits_add_item(&app->credits, "DNI Web Design", app->gfont[FONT_GAME_OUTLINE], 768, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Support", app->gfont[FONT_GAME_OUTLINE], 864, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Brandy Cope", app->gfont[FONT_GAME_OUTLINE], 896, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Dawson Irvine", app->gfont[FONT_GAME_OUTLINE], 928, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Testing", app->gfont[FONT_GAME_OUTLINE], 1024, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Todd Cope", app->gfont[FONT_GAME_OUTLINE], 1056, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Brandy Cope", app->gfont[FONT_GAME_OUTLINE], 1088, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Additional Credits", app->gfont[FONT_GAME_OUTLINE], 1184, makecol(192, 192, 255));
	credits_add_item(&app->credits, "Allegro Developers", app->gfont[FONT_GAME_OUTLINE], 1216, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Ben Davis (DUMB)", app->gfont[FONT_GAME_OUTLINE], 1248, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Javier Gonzalez (AlFont)", app->gfont[FONT_GAME_OUTLINE], 1280, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Matthew Leverton", app->gfont[FONT_GAME_OUTLINE], 1312, makecol(255, 255, 255));
	credits_add_item(&app->credits, "SKALE Tracker Developers", app->gfont[FONT_GAME_OUTLINE], 1344, makecol(255, 255, 255));
	credits_add_item(&app->credits, "GoldWave Inc.", app->gfont[FONT_GAME_OUTLINE], 1376, makecol(255, 255, 255));
	credits_add_item(&app->credits, "Jasc Software", app->gfont[FONT_GAME_OUTLINE], 1408, makecol(255, 255, 255));

	credits_add_item(&app->credits, "Alphabet Soup (c) 2004-2005", app->gfont[FONT_GAME_OUTLINE], 1888, makecol(255, 255, 255));
	credits_add_item(&app->credits, "NCD Games. All Rights Reserved.", app->gfont[FONT_GAME_OUTLINE], 1920, makecol(255, 255, 255));

	return true;
}

void data_free(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int i;

	/* free the images */
	for(i = 0; i < MAX_IMAGES; i++)
	{
		if(app->image[i])
		{
			al_destroy_bitmap(app->image[i]);
		}
	}

	/* free the animations */
	for(i = 0; i < MAX_ANIMATIONS; i++)
	{
		if(app->animation[i])
		{
			destroy_ani(app->animation[i]);
		}
	}

	/* free the sounds */
	for(i = 0; i < MAX_SOUNDS; i++)
	{
		if(app->sound[i])
		{
			al_destroy_sample(app->sound[i]);
		}
	}

	/* free the fonts */
	for(i = 0; i < MAX_FONTS; i++)
	{
		if(app->gfont[i])
		{
			t3f_destroy_font(app->gfont[i]);
		}
	}

//	save_score("scores");
}
