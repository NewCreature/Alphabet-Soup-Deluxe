#ifndef AS_3D_CAN_H
#define AS_3D_CAN_H

#include "t3f/t3f.h"

typedef struct
{

	ALLEGRO_BITMAP * face_texture;
	ALLEGRO_BITMAP * top_texture;
	ALLEGRO_BITMAP * bottom_texture;
	ALLEGRO_VERTEX * face_vertex;
	ALLEGRO_VERTEX * top_vertex;
	ALLEGRO_VERTEX * bottom_vertex;
	float width;
	float height;

	float cx;
	float y;
	float z;
	int texture_pos;
	float old_cx;
	float old_y;
	float old_z;
	int old_texture_pos;

} AS_3D_CAN;

AS_3D_CAN * as_create_3d_can(ALLEGRO_BITMAP * face_texture, ALLEGRO_BITMAP * top_texture, ALLEGRO_BITMAP * bottom_texture, float width, float height);
void as_destroy_3d_can(AS_3D_CAN * cp);

void as_move_3d_can(AS_3D_CAN * cp, float cx, float y, float z, int texture_pos);
void as_render_3d_can(AS_3D_CAN * cp);

#endif
