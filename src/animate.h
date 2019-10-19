#ifndef ANIMATE_H
#define ANIMATE_H

#include "t3f/t3f.h"

#ifdef __cplusplus
extern "C"{
#endif

#define ANIMATION_MAX_FRAMES 256

#define ANIMATION_TYPE_LOOP    0
#define ANIMATION_TYPE_STICKY  1

/* animation structure */
typedef struct
{

    ALLEGRO_BITMAP * image[ANIMATION_MAX_FRAMES];
    int w; /* width */
    int h; /* height */
    int f; /* frames */
    int d; /* speed (in delay frames) */
    int type;

} ANIMATION;

typedef struct
{

	int r[256], g[256], b[256];

} PALETTE;

/* disk functions */
ANIMATION * load_ani_fp(ALLEGRO_FILE * fp, PALETTE * pal);
ANIMATION * load_ani(char * fn, PALETTE * pal);
ANIMATION * duplicate_ani(ANIMATION * ap);
int save_ani_fp(ANIMATION * ap, ALLEGRO_FILE * fp, PALETTE * pal);
int save_ani(ANIMATION * ap, char * fn, PALETTE * pal);
void destroy_ani(ANIMATION * ap);

/* used to construct animations (used in editor) */
void initialize_ani(ANIMATION * ap);
ANIMATION * create_ani(int w, int h);
ANIMATION * clip_ani(ANIMATION * ap, int x, int y, int w, int h);
ANIMATION * mask_ani(ANIMATION * ap, int x, int y, int w, int h);
int animation_add_frame(ANIMATION * ap, ALLEGRO_BITMAP * bp);
int animation_insert_frame(ANIMATION * ap, int f, ALLEGRO_BITMAP * bp);
int animation_replace_frame(ANIMATION * ap, int f, ALLEGRO_BITMAP * bp);
int animation_delete_frame(ANIMATION * ap, int f);
int animation_copy_frame(ANIMATION * ap, int f, ALLEGRO_BITMAP * bp);

/* helper functions for the programmer */
ALLEGRO_BITMAP * get_ani(ANIMATION * ap, int f);
ALLEGRO_BITMAP * get_ani_frame(ANIMATION * ap, int f);

/* drawing functions */
int blit_ani(ANIMATION * ap, ALLEGRO_BITMAP * bp, int x, int y, unsigned long f);
int blit_ani_frame(ANIMATION * ap, ALLEGRO_BITMAP * bp, int x, int y, int f);
int draw_ani(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, unsigned long f);
int draw_ani_frame(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, int f);
int draw_trans_ani(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, unsigned long f);
int draw_trans_ani_frame(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, unsigned long f);
int draw_solid_ani(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, ALLEGRO_COLOR color, unsigned long f);
int draw_solid_ani_frame(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, ALLEGRO_COLOR color, unsigned long f);

#ifdef __cplusplus
}
#endif

#endif
