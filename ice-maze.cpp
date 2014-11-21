#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#include "player.h"
#include "window.h"
#include "camera.h"
#include "level.h"

//Prototype Fuctions

//Init Functions
void init(int argc, char **argv);

//Main Functions
void update();
void draw();
void run();
void quit();

//Handle Input
void process_events();
void handle_key(SDL_Event event, int state);

//Computation function
double screen_to_world(double i, int dim);

//COLLISION FUNCTIONS
void set_player_attributes();
void check_player_collision();

//VARIABLES
//Display elements
Window *window;

//Game elements
Player *player;
Level *level;
Camera *camera;

//Input arrays
int key_down[4];

//FPS Variables

//Debug
bool debug = false;
std::ofstream debug_file;


int main(int argc, char **argv){
	init(argc, argv);
	run();
	return 0;
}

void init(int argc, char **argv){
	//Open Debug File
	debug_file.open("debug.txt", std::ios::app);
	debug_file << "---------------------------------------------" << std::endl;

	//Init window
	window = new Window(1400,700, false);
	if(!window->init()){
		exit(EXIT_FAILURE);
	}

	//Init level
	level = new Level(1);
	level->init_level();

	//Init player
	player = new Player();
	camera = new Camera(level->get_start_x(), level->get_start_y());

	key_down[0] = 0;
	key_down[1] = 0;
	key_down[2] = 0;
	key_down[3] = 0;
}

void run(){
	while(1){
		process_events();
		update();
		draw();
		//Switch buffers
		window->update();
	}
}

void update(){
	set_player_attributes();
	level->update();
	player->update();

	check_player_collision();

	camera->update(key_down);
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	camera->set();

	level->draw();
	player->draw();
}

void quit(){
	delete window;
	delete player;
	delete camera;
	delete level;

	exit(EXIT_SUCCESS);
}

//Events
void process_events(){
	SDL_Event event;

	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_KEYDOWN:
				handle_key(event, 1);
				break;
			case SDL_KEYUP:
				handle_key(event, 0);
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_LEFT){
					level->mouse_x = screen_to_world(event.button.x, 0);
					level->mouse_y = screen_to_world(event.button.y, 1);
					player->set_target_pos(level->mouse_x, level->mouse_y);
				}
				break;
			case SDL_QUIT:
				quit();
				break;
		}
	}
}

void handle_key(SDL_Event event, int state)
{
	switch(event.key.keysym.sym) {
		case SDLK_LEFT:
			key_down[0] = state;
			break;
		case SDLK_RIGHT:
			key_down[1] = state;
			break;
		case SDLK_UP:
			key_down[2] = state;
			break;
		case SDLK_DOWN:
			key_down[3] = state;
			break;
		case SDLK_F10:
			if (state)
				window->set_fullscreen();
			break;
		case SDLK_ESCAPE:
			quit();
			break;
		default:
			break;
	}
}

//COMPUTATION FUNCTIONS
double screen_to_world(double i, int dim){
	if(dim == 0)
		return camera->getX() + ((i - window->get_window_width() / 2) / (49.25));
	if(dim == 1)
		return camera->getY() - ((i - window->get_window_height() / 2) / (49.25));
	return 0;
}

/*
 * Gets the id of the player and returns the speed of the ice
 */
void set_player_attributes(){
	int floor_id = level->get_flood_id(floor(player->get_pos_x()), floor(player->get_pos_y()));

	if(floor_id == 2){
		player->set_max_speed_on_ice(0.05);
    }
    else if(floor_id == 1){
    	player->set_max_speed_on_ice(0.0);
    }
  	else if(floor_id == 0){
		player->death(level->get_start_x(), level->get_start_y());
    	player->set_reached_target_pos(true);
    }
}

void check_player_collision(){
	int key;
	if(level->check_unit_collision(player))
		player->death(level->get_start_x(), level->get_start_y());
	if (level->check_gate_collision(player))
		player->reset_displacement();
	if ((key = level->check_key_collision(player)) != -1)
		player->give_item(key);
}
