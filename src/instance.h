#ifndef APP_INSTANCE_H
#define APP_INSTANCE_H

#include "t3f/t3f.h"
#include "t3f/font.h"
#include "defines.h"
#include "data.h"
#include "menu.h"
#include "game.h"
#include "3d_can.h"

/* structure to hold all of our app-specific data */
typedef struct
{

	/* main data */
	T3F_VIEW * letterbox_view;
	int logic_ticks;
	int frame;
	char * mem_buffer;
	int mem_buffer_size;
	int configure;
	int music_vol;
	int sound_vol;
	int state;
	ALLEGRO_SAMPLE * sound[MAX_SOUNDS];
	ALLEGRO_BITMAP * image[MAX_IMAGES];
	T3F_FONT * gfont[MAX_FONTS];
	ANIMATION * animation[MAX_ANIMATIONS];
	GMENU menu;
	GMENU config_menu;
	HIGH_SCORES high_scores[MAX_GAME_MODES];
	DICTIONARY dictionary[MAX_DICTIONARIES];
	GAME game;
	ALLEGRO_COLOR menu_bg_color;

	/* menu data */
	AS_3D_CAN * can;
	int can_x, can_y, can_ofs;
	int menu_state;
	int score_state;
	int score_view;
	char word_file[MAX_WORD_FILES][1024];
	int word_files;
	int word_selected;
	int gtime;
	int wy;
	CREDITS credits;

	/* game data */
	int game_state;
	int bowl_x, bowl_y;
	int scores_x, scores_y;
	int score_place;
	int score_pos;
	TEXT_OBJECT text[MAX_TEXT];
	TEXT_EFFECT_OBJECT text_effect[MAX_TEXT_EFFECT];

} APP_INSTANCE;

#endif
