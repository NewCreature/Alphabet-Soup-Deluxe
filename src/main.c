#include "t3f/t3f.h"
#include "t3f/rng.h"
#include "instance.h"
#include "data.h"
#include "game.h"
#include "hs.h"

/* main logic routine */
void app_logic(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	char buffer[1024];

	switch(app->state)
	{
		case STATE_GAME:
		{
			game_control(app);
			game_logic(&app->game, data);
			break;
		}
		case STATE_MENU:
		{
			if(!t3f_key[ALLEGRO_KEY_S])
			{
				menu_logic(app);
			}
			break;
		}
		case STATE_SCORES:
		{
			scores_logic(app);
			break;
		}
	}
	if(t3f_key[ALLEGRO_KEY_F12])
	{
		al_save_bitmap(t3f_get_filename(t3f_data_path, "screen.png", buffer, 1024), al_get_backbuffer(t3f_display));
		t3f_key[ALLEGRO_KEY_F12] = 0;
	}
}

/* main rendering routine */
void app_render(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;

	t3f_adjust_view(app->letterbox_view, t3f_default_view->left, t3f_default_view->top, t3f_default_view->right - t3f_default_view->left, t3f_default_view->bottom - t3f_default_view->top, 320, 240, T3F_FORCE_ASPECT);
	switch(app->state)
	{
		case STATE_GAME:
		{
			game_render(&app->game, data);
			break;
		}
		case STATE_MENU:
		{
			menu_render(app);
			break;
		}
		case STATE_SCORES:
		{
			scores_render(app);
			break;
		}
	}
}

void load_scores(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int i, j;
	char section_buf[64];
	char key_buf[64];
	const char * val;

	for(i = 0; i < MAX_GAME_MODES; i++)
	{
		sprintf(section_buf, "Mode %d High Scores", i);
		for(j = 0; j < HS_MAX; j++)
		{
			sprintf(key_buf, "name_%d", j);
			val = al_get_config_value(t3f_config, section_buf, key_buf);
			if(val)
			{
				strcpy(app->high_scores[i].place[j].name, val);
			}
			else
			{
				strcpy(app->high_scores[i].place[j].name, "No Name");
			}
			sprintf(key_buf, "score_%d", j);
			val = al_get_config_value(t3f_config, section_buf, key_buf);
			if(val)
			{
				app->high_scores[i].place[j].score = atoi(val);
			}
			else
			{
				app->high_scores[i].place[j].score = 5000 - j * 500;
			}
		}
	}
}

void save_scores(void * data)
{
	APP_INSTANCE * app = (APP_INSTANCE *)data;
	int i, j;
	char section_buf[64];
	char key_buf[64];
	char buf[64];

	for(i = 0; i < MAX_GAME_MODES; i++)
	{
		sprintf(section_buf, "Mode %d High Scores", i);
		for(j = 0; j < HS_MAX; j++)
		{
			sprintf(key_buf, "name_%d", j);
			al_set_config_value(t3f_config, section_buf, key_buf, app->high_scores[i].place[j].name);
			sprintf(key_buf, "score_%d", j);
			sprintf(buf, "%d", app->high_scores[i].place[j].score);
			al_set_config_value(t3f_config, section_buf, key_buf, buf);
		}
	}
}

/* initialize our app, load graphics, etc. */
bool app_initialize(APP_INSTANCE * app, int argc, char * argv[])
{
//	memset(app, 0, sizeof(APP_INSTANCE));

	/* initialize T3F */
	if(!t3f_initialize(T3F_APP_TITLE, 640, 480, 60.0, app_logic, app_render, T3F_DEFAULT | T3F_FILL_SCREEN | T3F_RESIZABLE, app))
	{
		printf("Error initializing T3F\n");
		return false;
	}
	t3f_set_gfx_mode(960, 540, t3f_flags);
	app->letterbox_view = t3f_create_view(t3f_default_view->left, t3f_default_view->top, t3f_default_view->right, t3f_default_view->bottom, 320, 240, T3F_FORCE_ASPECT | T3F_RESIZABLE);
	if(!app->letterbox_view)
	{
		printf("Error creating view!\n");
		return false;
	}
	t3f_set_view_virtual_dimensions(app->letterbox_view, 640, 480);
	srand(time(0));
	if(!data_load(app))
	{
		printf("Error loading data!\n");
		return false;
	}

	/* set default options */
	app->logic_ticks = 0;
	app->frame = 0;
	app->mem_buffer = NULL;
	app->mem_buffer_size = 0;
	app->configure = 0;
	app->music_vol = 100;
	app->sound_vol = 50;
	app->state = STATE_MENU;

	config_load(app);
	load_scores(app);
	app->can_x = 640 / 2;
	app->can_y = CAN_END_Y;
	app->can_ofs = CAN_START_OFS;
	app->menu_state = MENU_STATE_TO_MENU;
	menu_init(app);
	t3f_play_music("data/music/title.xm");

	return true;
}

void app_exit(APP_INSTANCE * app)
{
	t3f_stop_music();
	config_save(app);
	save_scores(app);
	t3f_save_config();
	data_free(app);
}

int main(int argc, char * argv[])
{
	APP_INSTANCE * app = malloc(sizeof(APP_INSTANCE));
	if(!app)
	{
		return -1;
	}

	if(app_initialize(app, argc, argv))
	{
		t3f_run();
	}
	else
	{
		printf("Error: could not initialize T3F!\n");
	}
	app_exit(app);
	return 0;
}
