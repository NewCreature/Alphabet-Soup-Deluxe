#ifndef HS_H
#define HS_H

#include "t3f/t3f.h"

#define HS_MAX      10
#define HS_NAME_MAX 32

/* this structure holds a single score */
typedef struct
{

	int score;
	char name[HS_NAME_MAX];

} HIGH_SCORE;

/* contains a list of high scores */
typedef struct
{

	HIGH_SCORE place[HS_MAX];
	int lit;

} HIGH_SCORES;

int hs_save_fp(HIGH_SCORES * hp, ALLEGRO_FILE * fp);
int hs_save(HIGH_SCORES * hp, char * fn);
int hs_load_fp(HIGH_SCORES * hp, ALLEGRO_FILE * fp);
int hs_load(HIGH_SCORES * hp, char * fn);
void hs_default(HIGH_SCORES * hp);
void hs_insert(HIGH_SCORES * hp, int pos, int score, char * name);
int hs_place(HIGH_SCORES * hp, int score);
void hs_render(ALLEGRO_BITMAP * bp, HIGH_SCORES * hp, T3F_FONT * fp);
void hs_render_ex(ALLEGRO_BITMAP * bp, HIGH_SCORES * hp, T3F_FONT * fp, int ox, int oy, int margin, int vy, ALLEGRO_COLOR lit, ALLEGRO_COLOR unlit, ALLEGRO_COLOR shad);

#endif
