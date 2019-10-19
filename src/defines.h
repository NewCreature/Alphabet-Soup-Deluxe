#ifndef DEFINES_H
#define DEFINES_H

#define MAX_WORD_FILES 64
#define MAX_SOUNDS     256
#define MAX_IMAGES     256
#define MAX_FONTS       16
#define MAX_ANIMATIONS  64
#define MAX_TEXT        64
#define MAX_TEXT_EFFECT 64

#define STATE_LOGO   0
#define STATE_MENU   1
#define STATE_GAME   2
#define STATE_FIN    3
#define STATE_SCORES 4

#define CAN_R 120
#define CAN_H 360

#define MAX_DICTIONARIES 2
#define DICTIONARY_MAIN  0
#define DICTIONARY_PLAY  1

#define IMAGE_GAME_BACKGROUND 0
#define IMAGE_GAME_STATS      1
#define IMAGE_SOUP_CAN        2
#define IMAGE_SOUP_SHADOW     3
#define IMAGE_SOUP_LID        4
#define IMAGE_MENU_BG         5
#define IMAGE_MENU_SHAPE_1    6
#define IMAGE_MENU_SHAPE_2    7
#define IMAGE_MENU_SHAPE_3    8
#define IMAGE_MENU_SHAPE_4    9
#define IMAGE_MENU_SURFACE   10
#define IMAGE_PAPER          11
#define IMAGE_BODY_H         12
#define IMAGE_BODY_V         13
#define IMAGE_BODY_DUL       14
#define IMAGE_BODY_DUR       15
#define IMAGE_BODY_DDL       16
#define IMAGE_BODY_DDR       17
#define IMAGE_HEAD_UP        18
#define IMAGE_HEAD_DOWN      19
#define IMAGE_HEAD_LEFT      20
#define IMAGE_HEAD_RIGHT     21
#define IMAGE_TAIL_UP        22
#define IMAGE_TAIL_DOWN      23
#define IMAGE_TAIL_LEFT      24
#define IMAGE_TAIL_RIGHT     25

#define ANIMATION_APPEAR_BG  0
#define ANIMATION_APPEAR_FG  1
#define ANIMATION_SNAKE_HEAD 2
#define ANIMATION_SNAKE_BODY 3

#define SOUND_BITE            0
#define SOUND_MENU_MOVE       1
#define SOUND_MENU_ENTER      2
#define SOUND_MENU_OPTION     3
#define SOUND_WORD            4
#define SOUND_LEVEL           5
#define SOUND_PAPER           6
#define SOUND_CLAP            7

#define FONT_GAME             0
#define FONT_MENU_HEADER      1
#define FONT_MENU             2
#define FONT_GAME_BIG         3
#define FONT_GAME_OUTLINE     4
#define FONT_MENU_OUTLINE     5

#define SNAKE_MAX_SEGMENTS 256
#define MAX_SNAKES           4

#define MAP_W (12)
#define MAP_H (12)

#define MAX_LETTERS 26

#define GAME_STATE_IN          0
#define GAME_STATE_PLAY        1
#define GAME_STATE_OUT         2
#define GAME_STATE_DIE         3
#define GAME_STATE_QUIT        4
#define GAME_STATE_WIN         5
#define GAME_STATE_DIE2        6

#define MAX_GAME_MODES          9
#define GAME_MODE_SCATTER_WORD  0
#define GAME_MODE_BLIND         1
#define GAME_MODE_ELIMINATION   2
#define GAME_MODE_POISON        3
#define GAME_MODE_FAT           4
#define GAME_MODE_SKINNY        5
#define GAME_MODE_COLD          6
#define GAME_MODE_NIBBLES       7
#define GAME_MODE_TEST          8

#endif
