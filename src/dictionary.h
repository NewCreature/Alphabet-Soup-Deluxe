#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "snake.h"

#define WORD_MAX_LENGTH         16
#define DICTIONARY_MAX_WORDS 64000

typedef struct
{

	char letter[WORD_MAX_LENGTH];
	int found;

} WORD;

typedef struct
{

	WORD word[DICTIONARY_MAX_WORDS];
	int words;
	int order[WORD_MAX_LENGTH + 1][4096];
	int ocount[WORD_MAX_LENGTH + 1];

	char current_word[WORD_MAX_LENGTH];
	int word_min, word_max;

} DICTIONARY;

void dictionary_add(DICTIONARY * dp, char * word, int opt);

void dictionary_sort(DICTIONARY * dp);
void dictionary_load(DICTIONARY * dp, char * fn, int opt);
void dictionary_save(DICTIONARY * dp, char * fn);
int  dictionary_find(DICTIONARY * dp, SNAKE * sp);
char * dictionary_rand_word(DICTIONARY * dp, int lmin, int lmax);
char * dictionary_rand_word_take(DICTIONARY * dp, int lmin, int lmax);
void dictionary_rand(DICTIONARY * dp1, DICTIONARY * dp2, int count, int min, int max);

#endif
