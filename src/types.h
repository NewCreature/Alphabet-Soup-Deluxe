#ifndef TYPES_H
#define TYPES_H

#include "defines.h"

typedef struct
{

	int left, right, up, down, fire, wfire, rotate;
	int left_key, right_key, up_key, down_key, fire_key, rotate_key;

	int gamepad;

} CONTROLLER;

typedef struct
{

	char letter;
	int x, y, nx, ny, px, py;
	int marked;

} SEGMENT;

typedef struct
{

	SEGMENT segment[SNAKE_MAX_SEGMENTS];
	char string[SNAKE_MAX_SEGMENTS + 1];
	int length;
	int score;
	int words;
	int delay, left, speed;
	CONTROLLER controller;

	char * word;
	int word_pos;

} SNAKE;

typedef struct
{

	int x, y, f, active, time;
	int val;

} LETTER;

typedef struct
{

	/* tell which map positions are blocked */
	int blocked[MAP_H][MAP_W];

	/* letter tiles */
	LETTER letter[26];

	/* use these when placing a new letter tile */
	int list[MAP_H * MAP_W];
	int list_size;

} MAP;

typedef struct
{

	MAP map;
	SNAKE snake;
	int mode;
	int difficulty;
	int level;
	int speed;
	int density;
	int word_min, word_max;
	int word_inc;
	int word_pos;

} GAME;

#endif
