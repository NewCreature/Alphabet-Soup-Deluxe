#include "includes.h"

void read_controller(CONTROLLER * cp)
{
	if(t3f_key[cp->left_key])
	{
		cp->left = 1;
	}
	else
	{
		cp->left = 0;
	}
	if(t3f_key[cp->right_key])
	{
		cp->right = 1;
	}
	else
	{
		cp->right = 0;
	}
	if(t3f_key[cp->up_key])
	{
		cp->up = 1;
	}
	else
	{
		cp->up = 0;
	}
	if(t3f_key[cp->down_key])
	{
		cp->down = 1;
	}
	else
	{
		cp->down = 0;
	}
	if(t3f_key[cp->fire_key])
	{
		cp->fire = 1;
		t3f_key[cp->fire_key] = 0;
	}
	else
	{
		cp->fire = 0;
	}
	if(t3f_key[cp->rotate_key])
	{
		cp->rotate = 1;
		t3f_key[cp->rotate_key] = 0;
	}
	else
	{
		cp->rotate = 0;
	}
}
