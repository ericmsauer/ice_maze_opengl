#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>

class Player
{
	public:
		Player();
		~Player();

		void update();
		void draw();

		//Event
		void death(double x, double y);
		void reset_position();

		//Movement
		void update_direction();
		void update_displacement();
		void reset_displacement();

		//Set and Get Functions
		double get_pos_x(){ return this->pos_x; }
		double get_pos_y(){ return this->pos_y; }
		double get_radius(){ return this->radius; }

		void set_pos(double _x, double _y){ this->pos_x = _x;
											this->pos_y = _y; }
		void set_target_pos(double _x, double _y){ this->target_pos_x = _x;
												   this->target_pos_y = _y;
												   this->reached_target_pos = false; }
		void set_max_speed_on_ice(double value){
			this->max_speed_on_ice = value;
		}

		void set_reached_target_pos(bool value){
			this->reached_target_pos = value;
		}

		bool give_item(int id);
		bool remove_item(int id);
		bool has_item(int id);
	private:
		//Final
		const double MOVEMENT_EPSILON = .05;

		//Debug
		bool debug;

		//----------- Movement Variables ------------
		double target_pos_x, target_pos_y;
		double pos_x, pos_y, pos_z;
		double speed = 0.0;
		double max_speed = 0.02;
		double max_speed_on_ground = .02;
		double max_speed_on_ice;
		double acc = 0.0;

		double direction = 0.0;
		double target_direction = 0.0;
		double rotation_speed = 0.2;

		bool reached_target_pos = true;

		//Attributes
		double radius = .2;

		//Items
		int *items;
		int num_items = 0;
		int max_items = 5;

		//Helper functions
		bool almost_equal(double first, double second, double EPISON){
			if(first + EPISON >= second && first - EPISON <= second)
				return true;
			return false;
		}
};
#endif
