#ifndef __GATE_H__
#define __GATE_H__

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>

class Gate
{
public:
	Gate(int _keyid, double _x_1, double _y_1, double _x_2, double _y_2);
	~Gate();

	void draw();

	double get_x_1(){ return this->x_1; }
	double get_y_1(){ return this->y_1; }
	double get_x_2(){ return this->x_2; }
	double get_y_2(){ return this->y_2; }

	double get_thickness(){ return this->thickness; }

	int get_keyid(){ return this->keyid; }

	void open_gate(){ this->open = true; }
	bool is_open(){ return this->open; }

private:
	double x_1, x_2, y_1, y_2;

	int keyid;
	bool open = false;

	//Attributes
	double thickness = .2;
};
#endif
