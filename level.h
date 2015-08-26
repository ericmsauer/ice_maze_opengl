#ifndef __LEVEL_H__
#define __LEVEL_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include "unit.h"
#include "gate.h"
#include "player.h"
#include "key.h"

extern std::ofstream debug_file;

class Level
{
	public:
		Level(int level);
		~Level();

		bool init_level();

		void update();
		void draw();

		bool check_unit_collision(Player *player);
		bool check_gate_collision(Player *player);
		int check_key_collision(Player *player);

		int get_height(){ return this->height; }
		int get_width(){ return this->width; }

		int get_start_x(){ return this->start_x; }
		int get_start_y(){ return this->start_y; }

		int get_flood_id(int x, int y){
			return floor_grid[x][y];
		}

		//Mouse Input
		double mouse_x = 0.0;
		double mouse_y = 0.0;
	private:
		int level_num;
		//Level attributes
		int start_x = -1;
		int start_y = -1;
		int end_x = -1;
		int end_y = -1;
		int height = -1;
		int width = -1;

		int **floor_grid = NULL;

		int num_units = 0;
		Unit **units = NULL;

		int num_gates = 0;
		Gate **gates = NULL;

		int num_keys = 0;
		Key **keys = NULL;

		bool init_grid(int level);
		bool init_units(int level);
		bool init_gates(int level);
		bool init_keys(int level);

		void draw_floor();
		void draw_terrian();
		void draw_grid();
		void draw_click();

		bool check_player_unit_collision(Player *player, Unit *unit);
		bool check_player_key_collision(Player *player, Key *unit);
		bool check_player_gate_collision(Player *player, Gate *gate);

		//Math functions
		double distance(double x_1, double y_1, double x_2, double y_2);
};
#endif
