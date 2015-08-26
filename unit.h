#ifndef __UNIT_H__
#define __UNIT_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>

class Unit
{
	public:
		Unit(int _id, double *_path_x, double *_path_y, int _nodes);
		~Unit();

		void update();
		void draw();

		double get_pos_x(){ return this->pos_x; }
		double get_pos_y(){ return this->pos_y; }
		double get_radius(){ return this->radius; }

		//Movement
		void update_direction();
		void update_displacement();

	private:
		//Final
		const double MOVEMENT_EPSILON = .05;

		//----------- Movement Variables ------------
		double *path_x, *path_y;
		int current_node, nodes;

		double target_pos_x, target_pos_y;
		double pos_x, pos_y, pos_z;
		double speed = 0.0;
		double max_speed = 0.04;
		double acc = 0.01;

		double direction = 0.0;
		double target_direction = 0.0;
		double rotation_speed = M_PI;

		bool reached_target_pos = true;

		//Attributes
		double radius = .2;

		//Helper functions
		bool almost_equal(double first, double second, double EPISON){
			if(first + EPISON >= second && first - EPISON <= second)
				return true;
			return false;
		}
};
#endif
