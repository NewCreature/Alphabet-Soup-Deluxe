#ifndef MENU_H
#define MENU_H

#include "t3f/font.h"

#define MENU_STATE_MAIN         0
#define MENU_STATE_TO_GAME      1
#define MENU_STATE_TO_OPTIONS   2
#define MENU_STATE_OPTIONS      3
#define MENU_STATE_TO_MAIN      4
#define MENU_STATE_TO_MENU      5
#define MENU_STATE_TO_OS        6
#define MENU_STATE_TO_SCORES    7
#define MENU_STATE_FROM_SCORES  8
#define MENU_STATE_CONFIG       9
#define MENU_STATE_CREDITS     10

#define SCORE_STATE_VIEW        0
#define SCORE_STATE_ENTER       1
#define SCORE_STATE_IN          2
#define SCORE_STATE_OUT         3
#define SCORE_STATE_ENTER_IN    4

//#define CAN_START_OFS (image[IMAGE_SOUP_CAN]->w / 4 + image[IMAGE_SOUP_CAN]->w / 2 - 8)
#define CAN_START_OFS (al_get_bitmap_width(app->image[IMAGE_SOUP_CAN]) / 2)
#define CAN_START_Y (480 / 2 - app->can->height / 2)
#define CAN_END_Y (-620)

#define MENU_ITEM_TYPE_CHOICE   0
#define MENU_ITEM_TYPE_TEXT     1
#define MENU_ITEM_TYPE_BAR      2

#define MENU_MAX_ITEMS         32

typedef struct
{

	int type;
	int ox, oy;
	void(*left_proc)(void *), (*right_proc)(void *), (*enter_proc)(void *);
	T3F_FONT * font;
	void * data;
	char * help;

} MENU_ITEM;

typedef struct
{

	MENU_ITEM item[MENU_MAX_ITEMS];
	int items;
	int selected;

} GMENU;

extern int menu_state;

void config_init(void * data);
void menu_init(void * data);
void menu_logic(void * data);
void menu_render(void * data);
void scores_logic(void * data);
void scores_render(void * data);

#endif
