#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

class Camera
{
	public:
		Camera(int x, int y);
		~Camera();

		void update(int key_down[]);

		void set();

		double getX(){ return this->x; }
		double getY(){ return this->y; }

		double screen_to_world(double i, int dim);

	private:
		//Position Variables
		double x,y;
		double speed;
};
#endif
