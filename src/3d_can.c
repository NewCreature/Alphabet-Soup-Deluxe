#include "t3f/t3f.h"
#include "3d_can.h"

AS_3D_CAN * as_create_3d_can(ALLEGRO_BITMAP * face_texture, ALLEGRO_BITMAP * top_texture, ALLEGRO_BITMAP * bottom_texture, float width, float height)
{
	AS_3D_CAN * cp;
	float angle = 0.0 + ALLEGRO_PI / 16.0;
	float v_angle = (ALLEGRO_PI - (2.0 * (ALLEGRO_PI / 16.0))) / width;
	int size;
	int i;

	cp = malloc(sizeof(AS_3D_CAN));
	if(!cp)
	{
		goto fail;
	}
	memset(cp, 0, sizeof(AS_3D_CAN));
	cp->face_texture = face_texture;
	cp->top_texture = top_texture;
	cp->bottom_texture = bottom_texture;

	size = sizeof(ALLEGRO_VERTEX) * 6 * width;
	cp->face_vertex = malloc(size);
	if(!cp->face_vertex)
	{
		goto fail;
	}
	memset(cp->face_vertex, 0, size);

	cp->face_angle_cos = malloc(sizeof(float) * (size + 1));
	if(!cp->face_angle_cos)
	{
		goto fail;
	}
	angle = 0.0 + ALLEGRO_PI / 16.0;
	for(i = 0; i < size + 1; i++)
	{
		cp->face_angle_cos[i] = cos(angle);
		angle += v_angle;
	}

	cp->face_angle_z = malloc(sizeof(float) * (size + 1));
	if(!cp->face_angle_z)
	{
		goto fail;
	}
	angle = 0.0 + ALLEGRO_PI / 16.0;
	for(i = 0; i < size + 1; i++)
	{
		cp->face_angle_z[i] = cos(angle + ALLEGRO_PI / 2.0) * (width / 2.0);
		angle += v_angle;
	}

	cp->top_vertex = malloc(sizeof(ALLEGRO_VERTEX) * 130);
	if(!cp->top_vertex)
	{
		goto fail;
	}

	cp->bottom_vertex = malloc(sizeof(ALLEGRO_VERTEX) * 130);
	if(!cp->bottom_vertex)
	{
		goto fail;
	}

	cp->width = width;
	cp->height = height;

	return cp;

	fail:
	{
		as_destroy_3d_can(cp);
	}
	return NULL;
}

void as_destroy_3d_can(AS_3D_CAN * cp)
{
	if(cp)
	{
		if(cp->face_angle_z)
		{
			free(cp->face_angle_z);
		}
		if(cp->face_angle_cos)
		{
			free(cp->face_angle_cos);
		}
		if(cp->face_vertex)
		{
			free(cp->face_vertex);
		}
		if(cp->top_vertex)
		{
			free(cp->top_vertex);
		}
		if(cp->bottom_vertex)
		{
			free(cp->bottom_vertex);
		}
		free(cp);
	}
}

static void make_quad(float cx, float y, float oz, int width, int height, int section, float angle, float v_angle, ALLEGRO_VERTEX * vertex, float * a_cos, float * a_z)
{
	ALLEGRO_COLOR color;
	float c;
	float shade = 0.5;
	float gradient = 0.5;
	float final_shade;
	float fore_portion = width / 4;
	float aft_portion = width - fore_portion;
	float half_width = width / 2;

	/* calculate color */
	if(section < width / 4)
	{
		c = (float)section / ((float)width / 4.0);
		final_shade = shade + gradient * c;
		color = al_map_rgb_f(final_shade, final_shade, final_shade);
	}
	else
	{
		c = ((float)section - ((float)width / 4.0)) / aft_portion;
		final_shade = shade + gradient * (1.0 - c);
		color = al_map_rgb_f(final_shade, final_shade, final_shade);
	}

	vertex[0].x = t3f_project_x(cx - half_width * a_cos[section], oz + a_z[section]);
	vertex[0].y = t3f_project_y(y, oz + a_z[section]);
	vertex[0].z = 0;
	vertex[0].color = color;
	vertex[1].x = t3f_project_x(cx - half_width * a_cos[section], oz + a_z[section]);
	vertex[1].y = t3f_project_y(y + height - 1, oz + a_z[section]);
	vertex[1].z = 0;
	vertex[1].color = color;
	vertex[2].x = t3f_project_x(cx - half_width * a_cos[section + 1], oz + a_z[section + 1]);
	vertex[2].y = t3f_project_y(y, oz + a_z[section]);
	vertex[2].z = 0;
	vertex[2].color = color;
	vertex[3].x = t3f_project_x(cx - half_width * a_cos[section + 1], oz + a_z[section + 1]);
	vertex[3].y = t3f_project_y(y, oz + a_z[section]);
	vertex[3].z = 0;
	vertex[3].color = color;
	vertex[4].x = t3f_project_x(cx - half_width * a_cos[section], oz + a_z[section]);
	vertex[4].y = t3f_project_y(y + height - 1, oz + a_z[section]);
	vertex[4].z = 0;
	vertex[4].color = color;
	vertex[5].x = t3f_project_x(cx - half_width * a_cos[section + 1], oz + a_z[section + 1]);
	vertex[5].y = t3f_project_y(y + height - 1, oz + a_z[section]);
	vertex[5].z = 0;
	vertex[5].color = color;
}

static void update_quad_texture(ALLEGRO_BITMAP * texture, float texture_offset, int section, ALLEGRO_VERTEX * vertex)
{
	vertex[0].u = texture_offset + section;
	vertex[0].v = 0;
	vertex[1].u = texture_offset + section;
	vertex[1].v = al_get_bitmap_height(texture) - 1;
	vertex[2].u = texture_offset + section + 1;
	vertex[2].v = 0;
	vertex[3].u = texture_offset + section + 1;
	vertex[3].v = 0;
	vertex[4].u = texture_offset + section;
	vertex[4].v = al_get_bitmap_height(texture) - 1;
	vertex[5].u = texture_offset + section + 1;
	vertex[5].v = al_get_bitmap_height(texture) - 1;
}

static void update_end(ALLEGRO_VERTEX * vp, int vc, float cx, float y, float cz, float width)
{
	float angle;
	float v_angle;
	int i;
	float a_cos;
	float z;

	angle = 0.0;
	v_angle = (ALLEGRO_PI * 2.0) / (float)(vc - 2);
	vp[0].x = t3f_project_x(cx, cz);
	vp[0].y = t3f_project_y(y, cz);
	vp[0].z = 0;
	vp[0].color = al_map_rgb(96, 96, 96);
	for(i = 0; i < vc - 1; i++)
	{
		a_cos = cos(angle) * (width / 2);
		z = cz + cos(angle + ALLEGRO_PI / 2.0) * (width / 2);
		vp[i + 1].x = t3f_project_x(cx - a_cos, z);
		vp[i + 1].y = t3f_project_y(y, z);
		vp[i + 1].z = 0;
		vp[i + 1].color = al_map_rgb(96, 96, 96);
		angle += v_angle;
	}
}

void as_move_3d_can(AS_3D_CAN * cp, float cx, float y, float z, int texture_pos)
{
	float angle = 0.0 + ALLEGRO_PI / 16.0;
	float v_angle;
	int i;

	cp->old_cx = cp->cx;
	cp->old_y = cp->y;
	cp->old_z = cp->z;
	cp->old_texture_pos = cp->texture_pos;

	cp->cx = cx;
	cp->y = y;
	cp->z = z;
	cp->texture_pos = texture_pos;

	if(fabs(cp->cx - cp->old_cx) > 0.1 || fabs(cp->y - cp->old_y) > 0.1 || fabs(cp->z - cp->old_z) > 0.1)
	{
		v_angle = (ALLEGRO_PI - (2.0 * (ALLEGRO_PI / 16.0))) / cp->width;
		for(i = 0; i < cp->width; i++)
		{
			make_quad(cp->cx, cp->y, cp->z, cp->width, cp->height, i, angle, v_angle, &cp->face_vertex[i * 6], cp->face_angle_cos, cp->face_angle_z);
			angle += v_angle;
		}
		update_end(cp->top_vertex, 130, cp->cx, cp->y, cp->z, cp->width);
		update_end(cp->bottom_vertex, 130, cp->cx, cp->y + cp->height - 1, cp->z, cp->width);
	}
	if(cp->texture_pos != cp->old_texture_pos)
	{
		for(i = 0; i < cp->width; i++)
		{
			update_quad_texture(cp->face_texture, cp->texture_pos, i, &cp->face_vertex[i * 6]);
		}
	}
}

void as_render_3d_can(AS_3D_CAN * cp)
{
	al_draw_prim(cp->top_vertex, NULL, NULL, 0, 130, ALLEGRO_PRIM_TRIANGLE_FAN);
	al_draw_prim(cp->bottom_vertex, NULL, NULL, 0, 130, ALLEGRO_PRIM_TRIANGLE_FAN);
	al_draw_prim(cp->face_vertex, NULL, cp->face_texture, 0, cp->width * 6, ALLEGRO_PRIM_TRIANGLE_LIST);
}
