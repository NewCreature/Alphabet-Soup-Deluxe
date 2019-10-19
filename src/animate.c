#include "t3f/t3f.h"
#include "animate.h"

static PALETTE * last_palette = NULL;

/* free an animation from memory */
void destroy_ani(ANIMATION * ap)
{
    int i;

    /* destroy it */
    if(ap != NULL)
    {
        /* destroy the bitmaps */
        for(i = 0; i < ap->f; i++)
        {
            if(ap->image[i] != NULL)
            {
                al_destroy_bitmap(ap->image[i]);
            }
        }

        /* free the structure */
        free(ap);
        ap = NULL;
    }
}

/* load animation from a file pointer */
ANIMATION * load_ani_fp(ALLEGRO_FILE * fp, PALETTE * pal)
{
    ALLEGRO_STATE old_state;
    ALLEGRO_TRANSFORM identity;
    ALLEGRO_COLOR color;
    ANIMATION * ap;
    char header[4];
    int i, j, k, c;

    /* check format */
    al_fread(fp, header, 4);
    if(header[0] != 'A' || header[1] != 'N' || header[2] != 'I' || header[3] != 21)
    {
        return NULL;
    }

    ap = (ANIMATION *)malloc(sizeof(ANIMATION));

    /* load header */
    ap->w = al_fread32le(fp);
    ap->h = al_fread32le(fp);
    ap->f = al_fread32le(fp);
    ap->d = al_fread32le(fp);
    ap->type = al_fread32le(fp);

    /* load palette data */
    j = al_fgetc(fp);
    if(j)
    {
        if(pal != NULL)
        {
            last_palette = pal;
            for(i = 0; i < 256; i++)
            {
                pal->r[i] = al_fgetc(fp);
                pal->g[i] = al_fgetc(fp);
                pal->b[i] = al_fgetc(fp);
            }
        }
        else
        {
            for(i = 0; i < 256 * 3; i++)
            {
                al_fgetc(fp);
            }
        }
    }

    /* load animation data */
    al_identity_transform(&identity);
    for(i = 0; i < ap->f; i++)
    {
        ap->image[i] = al_create_bitmap(ap->w, ap->h);
        al_lock_bitmap(ap->image[i], ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_WRITEONLY);
        al_store_state(&old_state, ALLEGRO_STATE_TRANSFORM | ALLEGRO_STATE_TARGET_BITMAP);
        al_set_target_bitmap(ap->image[i]);
        al_use_transform(&identity);
        for(j = 0; j < ap->h; j++)
        {
            for(k = 0; k < ap->w; k++)
            {
                if(last_palette)
                {
                    c = al_fgetc(fp);
                    color = al_map_rgba_f(last_palette->r[c] / 63.0, last_palette->g[c] / 63.0, last_palette->b[c] / 63.0, 1.0);
                    if(c == 0)
                    {
                        color = al_map_rgba_f(0, 0, 0, 0);
                    }
                    al_put_pixel(k, j, color);
                }
            }
        }
        al_restore_state(&old_state);
        al_unlock_bitmap(ap->image[i]);
    }

    return ap;
}

/* load an animation from a filename */
ANIMATION * load_ani(char * fn, PALETTE * pal)
{
    ALLEGRO_FILE * fp;
    ANIMATION * ap;

    /* open the file */
    fp = al_fopen(fn, "rb");
    if(fp == NULL)
    {
        printf("a2\n");
        return NULL;
    }

    /* read the animation data */
    ap = load_ani_fp(fp, pal);
    al_fclose(fp);

    return ap;
}

ANIMATION * duplicate_ani(ANIMATION * oap)
{
	ANIMATION * ap;
	int i;

	ap = (ANIMATION *)malloc(sizeof(ANIMATION));

    /* load header */
    ap->w = oap->w;
    ap->h = oap->h;
    ap->f = oap->f;
    ap->d = oap->d;

    /* load animation data */
    for(i = 0; i < ap->f; i++)
    {
        ap->image[i] = al_clone_bitmap(oap->image[i]);
    }

    return ap;
}

/* save animation to a file pointer */
int save_ani_fp(ANIMATION * ap, ALLEGRO_FILE * fp, PALETTE * pal)
{
    return 0;
}

/* save an animation */
int save_ani(ANIMATION * ap, char * fn, PALETTE * pal)
{
    return 0;
}

/* create an empty animation of the specified width and height */
ANIMATION * create_ani(int w, int h)
{
    ANIMATION * ap;

    ap = (ANIMATION *)malloc(sizeof(ANIMATION));

    ap->w = w;
    ap->h = h;
    ap->f = 0;
    ap->d = 1;

    return ap;
}

/* clip a portion of an animation */
ANIMATION * clip_ani(ANIMATION * ap, int x, int y, int w, int h)
{
/*	ANIMATION * rap;
	int i;

	rap = create_ani(w, h);
	rap->f = ap->f;
	rap->d = ap->d;
	for(i = 0; i < ap->f; i++)
	{
		rap->image[i] = create_bitmap(w, h);
		if(!rap->image[i])
		{
			return NULL;
		}
		clear_bitmap(rap->image[i]);
		draw_sprite(rap->image[i], ap->image[i], -x, -y);
	}
	return rap; */
    return NULL;
}

/* mask a portion of an animation */
ANIMATION * mask_ani(ANIMATION * ap, int x, int y, int w, int h)
{
/*	ANIMATION * rap;
	BITMAP * scratch;
	int i;

	rap = create_ani(ap->w, ap->h);
	rap->f = ap->f;
	rap->d = ap->d;
	scratch = create_bitmap(w, h);
	for(i = 0; i < ap->f; i++)
	{
		rap->image[i] = create_bitmap(ap->w, ap->h);
		if(!rap->image[i])
		{
			return NULL;
		}
		clear_bitmap(scratch);
		draw_sprite(scratch, ap->image[i], -x, -y);
		draw_sprite(rap->image[i], scratch, x, y);
	}
	destroy_bitmap(scratch);
	return rap; */
    return NULL;
}

/* add frame to end of animation */
int animation_add_frame(ANIMATION * ap, ALLEGRO_BITMAP * bp)
{
    if(ap->f < ANIMATION_MAX_FRAMES && ap != NULL)
    {
        /* create the new frame */
        ap->image[ap->f] = bp;
        ap->f++;
        return 1;
    }
    return 0;
}

/* insert a frame of animation */
int animation_insert_frame(ANIMATION * ap, int f, ALLEGRO_BITMAP * bp)
{
    int i;

    /* make sure we are inserting image to appropriate place */
    if(f < ap->f && ap->f < ANIMATION_MAX_FRAMES && ap != NULL)
    {
        /* make room for the inserted frame */
        for(i = ap->f; i > f; i--)
        {
            ap->image[i] = ap->image[i - 1];
        }

        /* insert the new image */
        ap->image[f] = bp;
        ap->f++;

        return 1;
    }
    return 0;
}

int animation_replace_frame(ANIMATION * ap, int f, ALLEGRO_BITMAP * bp)
{
    if(f < ap->f && ap != NULL)
    {
        al_destroy_bitmap(ap->image[f]);
        ap->image[f] = bp;
        return 1;
    }
    return 0;
}

int animation_delete_frame(ANIMATION * ap, int f)
{
    int i;

    if(f < ap->f && ap != NULL)
    {
        /* move other frames back one position */
        al_destroy_bitmap(ap->image[f]);
        for(i = f; i < ap->f - 1; i++)
        {
            ap->image[i] = ap->image[i + 1];
        }

        /* adjust frame counter */
        ap->f--;

        return 1;
    }
    return 0;
}

int animation_copy_frame(ANIMATION * ap, int f, ALLEGRO_BITMAP * bp)
{
/*    if(f < ap->f && ap != NULL)
    {
        bp = create_bitmap(ap->w, ap->h);
        blit(ap->image[f], bp, 0, 0, 0, 0, ap->w, ap->h);

        return 1;
    } */
    return 0;
}

ALLEGRO_BITMAP * get_ani(ANIMATION * ap, int f)
{
    int i, d;

    switch(ap->type)
    {
	    case ANIMATION_TYPE_LOOP:
	    {
    		i = !ap->d ? f % ap->f : (f / ap->d) % ap->f;
    		break;
		}
	    case ANIMATION_TYPE_STICKY:
	    {
		    if(ap->d)
		    {
			    d = ap->d;
	    	}
	    	else
	    	{
		    	d = 1;
	    	}
	    	if(f < ap->f * d)
	    	{
    			i = (f / ap->d) % ap->f;
			}
			else
			{
				i = ap->f - 1;
			}
    		break;
		}
	}

    return ap->image[i];
}

ALLEGRO_BITMAP * get_ani_frame(ANIMATION * ap, int f)
{
    return ap->image[f];
}

/* draw the specified frame */
int blit_ani_frame(ANIMATION * ap, ALLEGRO_BITMAP * bp, int x, int y, int f)
{
    if(f < ap->f && ap != NULL)
    {
        al_draw_bitmap(ap->image[f], x, y, 0);

        return 1;
    }
    return 0;
}

/* similar to blit but for animations */
int blit_ani(ANIMATION * ap, ALLEGRO_BITMAP * bp, int x, int y, unsigned long f)
{
    int i;

    if(ap != NULL)
    {
        /* make sure there are frames to draw */
        if(ap->f > 0)
        {
            /* decide which frame to draw */
            i = !ap->d ? f % ap->f : (f / ap->d) % ap->f;
            al_draw_bitmap(ap->image[i], x, y, 0);

            return 1;
        }
    }
    return 0;
}

/* like draw_sprite but for animations (pick frame based on f) */
int draw_ani(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, unsigned long f)
{
    if(ap != NULL)
    {
        /* make sure there are frames to draw */
        if(ap->f > 0)
        {
            /* decide which frame to draw */
//            i = !ap->d ? f % ap->f : (f / ap->d) % ap->f;
            al_draw_bitmap(get_ani(ap, f), x, y, 0);

            return 1;
        }
    }
    return 0;
}

/* like draw_ani but doesn't calculate which frame to draw */
int draw_ani_frame(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, int f)
{
    if(ap != NULL)
    {
        if(f < ap->f)
        {
            al_draw_bitmap(ap->image[f], x, y, 0);

            return 1;
        }
    }
    return 0;
}

/* like draw_trans_sprite but for animations */
int draw_trans_ani(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, unsigned long f)
{
    int i;

    if(ap != NULL)
    {
        /* make sure there are frames to draw */
        if(ap->f > 0)
        {
            /* decide which frame to draw */
            i = !ap->d ? f % ap->f : (f / ap->d) % ap->f;
            al_draw_bitmap(ap->image[i], x, y, 0);

            return 1;
        }
    }
    return 0;
}

/* like draw_trans_ani but doesn't calculate which frame to draw */
int draw_trans_ani_frame(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, unsigned long f)
{
    if(ap != NULL)
    {
        if(f < ap->f)
        {
            al_draw_bitmap(ap->image[f], x, y, 0);

            return 1;
        }
    }
    return 0;
}

/* like draw_character but for animations */
int draw_solid_ani(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, ALLEGRO_COLOR color, unsigned long f)
{
    int i;

    if(ap != NULL)
    {
        /* make sure there are frames to draw */
        if(ap->f > 0)
        {
            /* decide which frame to draw */
            i = !ap->d ? f % ap->f : (f / ap->d) % ap->f;
            al_draw_tinted_bitmap(ap->image[i], color, x, y, 0);

            return 1;
        }
    }
    return 0;
}

/* like draw_solid_ani but doesn't calculate frame */
int draw_solid_ani_frame(ALLEGRO_BITMAP * bp, ANIMATION * ap, int x, int y, ALLEGRO_COLOR color, unsigned long f)
{
    if(ap != NULL)
    {
        if(f < ap->f)
        {
            al_draw_tinted_bitmap(ap->image[f], color, x, y, 0);

            return 1;
        }
    }
    return 0;
}
