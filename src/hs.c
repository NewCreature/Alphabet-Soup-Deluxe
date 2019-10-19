#include <string.h>
#include "t3f/t3f.h"
#include "hs.h"

int hs_save_fp(HIGH_SCORES * hp, ALLEGRO_FILE * fp)
{
	int i, j;

	for(i = 0; i < HS_MAX; i++)
	{
		for(j = 0; j < HS_NAME_MAX; j++)
		{
			al_fputc(fp, hp->place[i].name[j]);
		}
		al_fwrite32le(fp, hp->place[i].score);
	}
	return 1;
}

int hs_save(HIGH_SCORES * hp, char * fn)
{
	ALLEGRO_FILE * fp;

	fp = al_fopen(fn, "wb");
	if(!fp)
	{
		return 0;
	}
	hs_save_fp(hp, fp);
	al_fclose(fp);
	return 1;
}

int hs_load_fp(HIGH_SCORES * hp, ALLEGRO_FILE * fp)
{
	int i, j;

	for(i = 0; i < HS_MAX; i++)
	{
		for(j = 0; j < HS_NAME_MAX; j++)
		{
			hp->place[i].name[j] = al_fgetc(fp);
		}
		hp->place[i].score = al_fread32le(fp);
	}
	return 1;
}

int hs_load(HIGH_SCORES * hp, char * fn)
{
	ALLEGRO_FILE * fp;

	fp = al_fopen(fn, "rb");
	if(!fp)
	{
		hs_default(hp);
		return 1;
	}
	hs_load_fp(hp, fp);
	al_fclose(fp);
	return 1;
}

void hs_default(HIGH_SCORES * hp)
{
	int i;

	for(i = 0; i < HS_MAX; i++)
	{
		hp->place[i].score = 5000 - i * 500;
		strcpy(hp->place[i].name, "No Name");
	}
}

void hs_insert(HIGH_SCORES * hp, int pos, int score, char * name)
{
	int i;

	for(i = HS_MAX - 1; i > pos; i--)
	{
		hp->place[i].score = hp->place[i - 1].score;
		strcpy(hp->place[i].name, hp->place[i - 1].name);
	}
	hp->place[pos].score = score;
	strcpy(hp->place[pos].name, name);
}

int hs_place(HIGH_SCORES * hp, int score)
{
	int i;

	for(i = 0; i < 10; i ++)
	{
		if(score >= hp->place[i].score)
		{
			return i;
		}
	}
	return -1;
}

void hs_render(ALLEGRO_BITMAP * bp, HIGH_SCORES * hp, T3F_FONT * fp)
{
	int i;
	ALLEGRO_COLOR col;
	int end;
	char buffer[128];

	for(i = 0; i < HS_MAX; i++)
	{
		if(i == hp->lit || hp->lit < 0)
		{
			col = al_map_rgb(200, 200, 200);
		}
		else
		{
			col = al_map_rgb(100, 100, 100);
		}
		sprintf(buffer, "%d", hp->place[i].score);
		end = 640 - 160 - t3f_get_text_width(fp, buffer);
		t3f_draw_textf(fp, al_map_rgb(0, 0, 0), 160 + 2, i * 32 + 96 + 2, 0, 0, 0, 0, "%s", hp->place[i].name);
		t3f_draw_textf(fp, al_map_rgb(0, 0, 0), end + 2, i * 32 + 96 + 2, 0, 0, 0, 0, "%d", hp->place[i].score);
		t3f_draw_textf(fp, col, 160, i * 32 + 96, 0, 0, 0, 0, "%s", hp->place[i].name);
		t3f_draw_textf(fp, col, end, i * 32 + 96, 0, 0, 0, 0, "%d", hp->place[i].score);

	}
}

void hs_render_ex(ALLEGRO_BITMAP * bp, HIGH_SCORES * hp, T3F_FONT * fp, int ox, int oy, int margin, int vy, ALLEGRO_COLOR lit, ALLEGRO_COLOR unlit, ALLEGRO_COLOR shad)
{
	int i;
	ALLEGRO_COLOR col;
	int end;
	char buffer[128];

	for(i = 0; i < HS_MAX; i++)
	{
		if(i == hp->lit || hp->lit < 0)
		{
			col = lit;
		}
		else
		{
			col = unlit;
		}
		sprintf(buffer, "%d", hp->place[i].score);
		end = 640 - margin - t3f_get_text_width(fp, buffer);
//		if(shad >= 0)
		{
			t3f_draw_textf(fp, shad, ox + margin + 2, oy + i * vy + 96 + 2, 0, 0, 0, 0, "%s", hp->place[i].name);
			t3f_draw_textf(fp, shad, ox + end + 2, oy + i * vy + 96 + 2, 0, 0, 0, 0, "%d", hp->place[i].score);
		}
		t3f_draw_textf(fp, col, ox + margin, oy + i * vy + 96, 0, 0, 0, 0, "%s", hp->place[i].name);
		t3f_draw_textf(fp, col, ox + end, oy + i * vy + 96, 0, 0, 0, 0, "%d", hp->place[i].score);

	}
}
