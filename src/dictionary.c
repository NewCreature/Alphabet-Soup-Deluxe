#include <ctype.h>
#include "includes.h"

int quick_compare(char * src, char * dest, int length)
{
	int i;

	for(i = 0; i < length; i++)
	{
		if(src[i] != dest[i])
		{
			return 0;
		}
	}
	return 1;
}

int quick_find(char * src, SNAKE * sp)
{
	int i, j;

	if(strlen(sp->word) >= strlen(src))
	{
		for(i = 0; i < strlen(sp->word) - strlen(src); i++)
		{
			if(quick_compare(src, sp->word + i, strlen(src)))
			{
				for(j = i; j < i + strlen(src); j++)
				{
					sp->segment[j].marked = 1;
				}
				return 1;
			}
		}
	}
	return 0;
}

void dictionary_add(DICTIONARY * dp, char * word, int opt)
{
	int len;
	int i;

	/* if the word is already in the dictionary don't add it */
	if(opt)
	{
		for(i = 0; i < dp->words; i++)
		{
			if(!strcasecmp(dp->word[i].letter, word))
			{
				return;
			}
		}
	}
	strcpy(dp->word[dp->words].letter, word);
	dp->word[dp->words].found = 0;
	dp->words++;
	len = strlen(word);
	if(len < dp->word_min)
	{
		dp->word_min = len;
	}
	if(len > dp->word_max)
	{
		dp->word_max = len;
	}
}

void dictionary_delete(DICTIONARY * dp, int word)
{
	int i;

	for(i = word; i < dp->words - 1; i++)
	{
		strcpy(dp->word[i].letter, dp->word[i + 1].letter);
		dp->word[i].found = dp->word[i + 1].found;
	}
	dp->words--;
}

void dictionary_sort(DICTIONARY * dp)
{
	int i;

	for(i = 0; i < WORD_MAX_LENGTH; i++)
	{
		dp->ocount[i] = 0;
	}
	for(i = 0; i < dp->words; i++)
	{
		dp->order[strlen(dp->word[i].letter)][dp->ocount[strlen(dp->word[i].letter)]] = i;
		dp->ocount[strlen(dp->word[i].letter)]++;
	}
}

int pack_dgetc(ALLEGRO_FILE * fp)
{
	char letter = 0;

	while(letter < 'A' || letter > 'Z')
	{
		letter = al_fgetc(fp);
		if(letter == '\n' || al_feof(fp))
		{
			return 0;
		}
	}
	return letter;
}

void dictionary_load(DICTIONARY * dp, char * fn, int opt)
{
	ALLEGRO_FILE * fp;
	char word[128];
	int i;

	dp->words = 0;
	dp->word_min = 64;
	dp->word_max = 0;
	fp = al_fopen(fn, "rb");
	while(!al_feof(fp))
	{
		for(i = 0; i < 32; i++)
		{
			word[i] = al_fgetc(fp);
			if(word[i] == '\r' || word[i] == '\n' || word[i] == ' ')
			{
				word[i] = '\0';
//				pack_getc(fp);
				break;
			}
			else if(word[i] >= 'a' && word[i] <= 'z')
			{
				word[i] = toupper(word[i]);
			}
			else if(word[i] == '-')
			{
				if(i <= 0)
				{
					word[i] = '\0';
					break;
				}
			}
			else if(word[i] < 'A' || word[i] > 'Z')
			{
				word[i] = '\0';
				break;
			}
		}
		if(strlen(word) > 2)
		{
			dictionary_add(dp, word, opt);
		}
	}
	al_fclose(fp);
}

void dictionary_save(DICTIONARY * dp, char * fn)
{
	ALLEGRO_FILE * fp;
	int i;

	fp = al_fopen(fn, "wb");
	for(i = 0; i < dp->words; i++)
	{
		al_fputs(fp, dp->word[i].letter);
		al_fputc(fp, '\r');
		al_fputc(fp, '\n');
	}
/*	for(i = 15; i >= 3; i--)
	{
		for(j = 0; j < dp->ocount[i]; j++)
		{
			pack_fputs(dp->word[dp->order[i][j]].letter, fp);
			pack_putc('\r', fp);
			pack_putc('\n', fp);
		}
	} */
	al_fclose(fp);
}

int  dictionary_find(DICTIONARY * dp, SNAKE * sp)
{
	int i, j, l;
	int r = 0;

	for(i = 0; i < dp->words; i++)
	{
		if(strlen(sp->word) >= strlen(dp->word[i].letter))
		{
			for(j = 0; j < strlen(sp->word) - strlen(dp->word[i].letter) + 1; j++)
			{
				if(quick_compare(dp->word[i].letter, sp->word + j, strlen(dp->word[i].letter)))
				{
					for(l = j; l < j + strlen(dp->word[i].letter); l++)
					{
						sp->segment[l].marked = 1;
					}
					r = 1;
					dp->word[i].found = 1;
				}
			}
		}
	}
	return r;
}

int  dictionary_find_all(DICTIONARY * dp, SNAKE * sp)
{
	int i, j, l;
	int marked[SNAKE_MAX_SEGMENTS] = {0};
	int found[DICTIONARY_MAX_WORDS] = {0};

	for(i = 0; i < dp->words; i++)
	{
		if(strlen(sp->word) >= strlen(dp->word[i].letter))
		{
			for(j = 0; j < strlen(sp->word) - strlen(dp->word[i].letter) + 1; j++)
			{
				if(quick_compare(dp->word[i].letter, sp->word + j, strlen(dp->word[i].letter)))
				{
					for(l = j; l < j + strlen(dp->word[i].letter); l++)
					{
//						sp->segment[l].marked = 1;
						marked[l] = 1;
					}
//					dp->word[i].found = 1;
					found[i] = 1;
				}
			}
		}
	}
	for(i = 0; i < dp->words; i++)
	{
		if(!found[i])
		{
			return 0;
		}
	}
	for(i = 0; i < sp->length; i++)
	{
		sp->segment[i].marked = marked[i];
	}
	for(i = 0; i < dp->words; i++)
	{
		dp->word[i].found = found[i];
	}
	return 1;
}

char * dictionary_rand_word(DICTIONARY * dp, int lmin, int lmax)
{
	int list[DICTIONARY_MAX_WORDS];
	int list_length = 0;
	int i;

	for(i = 0; i < dp->words; i++)
	{
		if(strlen(dp->word[i].letter) >= lmin && strlen(dp->word[i].letter) <= lmax)
		{
			list[list_length] = i;
			list_length++;
		}
	}
	return dp->word[list[rand() % list_length]].letter;
}

char * dictionary_rand_word_take(DICTIONARY * dp, int lmin, int lmax)
{
	int list[DICTIONARY_MAX_WORDS];
	int list_length = 0;
	int i, pick;

	for(i = 0; i < dp->words; i++)
	{
		if(strlen(dp->word[i].letter) >= lmin && strlen(dp->word[i].letter) <= lmax)
		{
			list[list_length] = i;
			list_length++;
		}
	}
	pick = rand() % list_length;
	strcpy(dp->current_word, dp->word[list[pick]].letter);
	dictionary_delete(dp, pick);
	return dp->current_word;
}

void dictionary_rand(DICTIONARY * dp1, DICTIONARY * dp2, int count, int lmin, int lmax)
{
	int list[DICTIONARY_MAX_WORDS];
	int list_length = 0;
	int i, j, pick;

	for(i = 0; i < dp1->words; i++)
	{
		if(strlen(dp1->word[i].letter) >= lmin && strlen(dp1->word[i].letter) <= lmax)
		{
			list[list_length] = i;
			list_length++;
		}
	}

	dp2->words = 0;
	for(i = 0; i < count; i++)
	{
		pick = rand() % list_length;
		strcpy(dp2->word[dp2->words].letter, dp1->word[list[pick]].letter);
		dp2->word[dp2->words].found = 0;
		dp2->words++;
		for(j = pick; j < list_length - 1; j++)
		{
			list[j] = list[j + 1];
		}
		list_length--;
	}
}
