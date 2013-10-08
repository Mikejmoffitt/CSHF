#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "Fighter.h"
#include "Controller.h"
 
using namespace std;

// Screen dimension constants
const int WIDTH = 320;
const int HEIGHT = 240;

void parseSimpleInputs(Controller *pad1, Controller *pad2, ALLEGRO_KEYBOARD_STATE keyState)
{
	if (pad1->disabled == false)
	{
		// Process inputs for controller 1
		if (al_key_down(&keyState, pad1->keys[0]))
		{
			pad1->stick[0]++;
		}
		else
		{
			pad1->stick[0] = 0;
		}
		if (al_key_down(&keyState, pad1->keys[1]))
		{
			pad1->stick[1]++;
		}
		else
		{
			pad1->stick[1] = 0;
		}
		if (al_key_down(&keyState, pad1->keys[2]))
		{
			pad1->stick[2]++;
		}
		else
		{
			pad1->stick[2] = 0;
		}
		if (al_key_down(&keyState, pad1->keys[3]))
		{
			pad1->stick[3]++;
		}
		else
		{
			pad1->stick[3] = 0;
		}
		if (al_key_down(&keyState, pad1->keys[4]))
		{
			pad1->buttons[0]++;
		}
		else
		{
			pad1->buttons[0] = 0;
		}
		if (al_key_down(&keyState, pad1->keys[5]))
		{
			pad1->buttons[1]++;
		}
		else
		{
			pad1->buttons[1] = 0;
		}
		if (al_key_down(&keyState, pad1->keys[6]))
		{
			pad1->exbuttons[0]++;
		}
		else
		{
			pad1->exbuttons[0] = 0;
		}
		if (al_key_down(&keyState, pad1->keys[7]))
		{
			pad1->exbuttons[1]++;
		}
		else
		{
			pad1->exbuttons[1] = 0;
		}
	}
	// Process inputs for controller 2
	if (pad2->disabled == false)
	{
		if (al_key_down(&keyState, pad2->keys[0]))
		{
			pad2->stick[0]++;
		}
		else
		{
			pad2->stick[0] = 0;
		}
		if (al_key_down(&keyState, pad2->keys[1]))
		{
			pad2->stick[1]++;
		}
		else
		{
			pad2->stick[1] = 0;
		}
		if (al_key_down(&keyState, pad2->keys[2]))
		{
			pad2->stick[2]++;
		}
		else
		{
			pad2->stick[2] = 0;
		}
		if (al_key_down(&keyState, pad2->keys[3]))
		{
			pad2->stick[3]++;
		}
		else
		{
			pad2->stick[3] = 0;
		}
		if (al_key_down(&keyState, pad2->keys[4]))
		{
			pad2->buttons[0]++;
		}
		else
		{
			pad2->buttons[0] = 0;
		}
		if (al_key_down(&keyState, pad2->keys[5]))
		{
			pad2->buttons[1]++;
		}
		else
		{
			pad2->buttons[1] = 0;
		}
		
		if (al_key_down(&keyState, pad2->keys[6]))
		{
			pad2->exbuttons[0]++;
		}
		else
		{
			pad2->exbuttons[0] = 0;
		}
		if (al_key_down(&keyState, pad2->keys[7]))
		{
			pad2->exbuttons[1]++;
		}
		else
		{
			pad2->exbuttons[1] = 0;
		}
	}
}

void fixBounds(int *scroll, Fighter *player1, Fighter *player2)
{
	*scroll = (((player1->getX() + player2->getX())/2)-160)*-1;
	if (*scroll > 0)
	{
		*scroll = 0;
	}
	if (*scroll < -320)
	{
		*scroll = -320;
	}

}

void updateAndWait()
{
	al_flip_display();
	al_wait_for_vsync();
}

void startGame(ALLEGRO_DISPLAY *display, string p1, string p2)
{
	// Manually build a temporary backdrop graphic setup
	Sequence backdrop;
	Sequence backdrop2;
	backdrop.loadFrame("./data/background.png",32,0,0,false);
	backdrop2.loadFrame("./data/foreground.png",32,0,0,false);
	
	int scroll = 0;

    std::stringstream concat;
	std::string stuff;
	
	//Set up the keyboard state
	ALLEGRO_KEYBOARD_STATE keyState;

	// Load some temporary background music
	ALLEGRO_SAMPLE *demo;
	demo = al_load_sample("./data/music/ft.ogg");
	al_play_sample(demo, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);

	Fighter player1(64,220,"ryu",false,192,128);
	Fighter player2(128,220,"cfort",true,192,128);

	// Set up player pointers
	player1.setOpponent(&player2);
	player2.setOpponent(&player1);
	player1.input.setKeys(ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT,
		ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_Z, ALLEGRO_KEY_X, ALLEGRO_KEY_A, ALLEGRO_KEY_S);
	player2.input.setKeys(ALLEGRO_KEY_I, ALLEGRO_KEY_K, ALLEGRO_KEY_J,
		ALLEGRO_KEY_L, ALLEGRO_KEY_T, ALLEGRO_KEY_Y, ALLEGRO_KEY_G, ALLEGRO_KEY_H);
	while (240 < 480) // THIS IS THE MAIN LOOP GUYS THE REAL DEAL THE BEE'S KNEES
	{
		al_get_keyboard_state(&keyState);
		parseSimpleInputs(&(player1.input), &(player2.input), keyState);
		al_clear_to_color(al_map_rgb(0,0,0));
		player1.calcExtremes(scroll);
		player2.calcExtremes(scroll);
		player1.doPhysics();
		player2.doPhysics();
		fixBounds(&scroll, &player1, &player2);
		backdrop.blit(scroll,0,false);
		player1.blit(scroll);
		player2.blit(scroll);
		backdrop2.blit(scroll,0,false);
		updateAndWait();
	}
	al_destroy_sample(demo);
	al_destroy_display(display);
	al_uninstall_system();
}

int main(int argc, char *argv[])
{
	al_init();
	al_init_primitives_addon();
	al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
	// Initialize image addon
	al_init_image_addon();
	// Initialize audio
	al_install_audio();
	// Initialize keyboard
	al_install_keyboard();
	al_init_acodec_addon();
	al_set_window_title(display,"Moffitt Fighting Game II Turbo");
	// Set up various system whatsits
	al_reserve_samples(8);

	startGame(display, "adam", "cfort");
	return 0;
}