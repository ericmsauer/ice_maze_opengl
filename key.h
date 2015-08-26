#ifndef __KEY_H__
#define __KEY_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>

class Key
{
public:
	Key(int _keyid, double x, double y);
	~Key();

	void draw();

	double get_x(){ return this->x; }
	double get_y(){ return this->y; }
	double get_radius(){ return this->radius; }
	int get_id(){ return this->keyid; }

	void pickup_key(){ this->showing = false; }
	void drop_key(){ this->showing = true; }
private:
	double x, y;

	int keyid;
	bool showing = true;

	//Attributes
	double radius = .2;
};
#endif
