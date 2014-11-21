#include "key.h"

Key::Key(int _keyid, double _x, double _y){
	this->keyid = _keyid;
	this->x = _x;
	this->y = _y;
}

Key::~Key(){
}

void Key::draw(){
	if (this->showing){
		glPushMatrix();
		glTranslatef(this->x, this->y, 0);

		//Draw Base
		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-this->radius, -this->radius, 0);
		glVertex3f(this->radius, -this->radius, 0);
		glVertex3f(this->radius, this->radius, 0);
		glVertex3f(-this->radius, this->radius, 0);
		glEnd();

		glPopMatrix();
	}
}