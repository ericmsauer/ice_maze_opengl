#include "gate.h"

Gate::Gate(int _keyid, double _x_1, double _y_1, double _x_2, double _y_2){
	this->keyid = _keyid;
	this->x_1 = _x_1;
	this->y_1 = _y_1;
	this->x_2 = _x_2;
	this->y_2 = _y_2;
}

Gate::~Gate(){
}

void Gate::draw(){
	if (!this->open){
		if (this->x_1 == this->x_2){
			glPushMatrix();
			//glTranslatef(this->x_1, abs(this->y_1 - this->y_2), 0);

			glBegin(GL_QUADS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex3f(-this->thickness + this->x_1, this->y_1, 0);
			glVertex3f(this->thickness + this->x_1, this->y_1, 0);
			glVertex3f(this->thickness + this->x_1, this->y_2, 0);
			glVertex3f(-this->thickness + this->x_1, this->y_2, 0);
		}
		else{
			glPushMatrix();
			//glTranslatef(abs(this->x_1 - this->x_2), this->y_1, 0);

			glBegin(GL_QUADS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex3f(this->x_1, -this->thickness + this->y_1, 0);
			glVertex3f(this->x_1, this->thickness + this->y_1, 0);
			glVertex3f(this->x_2, this->thickness + this->y_1, 0);
			glVertex3f(this->x_2, -this->thickness + this->y_1, 0);
		}
	}
	else{
		if (this->x_1 == this->x_2){
			glPushMatrix();
			//glTranslatef(this->x_1, abs(this->y_1 - this->y_2), 0);

			glBegin(GL_QUADS);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(-this->thickness + this->x_1, this->y_1, 0);
			glVertex3f(this->thickness + this->x_1, this->y_1, 0);
			glVertex3f(this->thickness + this->x_1, this->y_2, 0);
			glVertex3f(-this->thickness + this->x_1, this->y_2, 0);
		}
		else{
			glPushMatrix();
			//glTranslatef(abs(this->x_1 - this->x_2), this->y_1, 0);

			glBegin(GL_QUADS);
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(this->x_1, -this->thickness + this->y_1, 0);
			glVertex3f(this->x_1, this->thickness + this->y_1, 0);
			glVertex3f(this->x_2, this->thickness + this->y_1, 0);
			glVertex3f(this->x_2, -this->thickness + this->y_1, 0);
		}
	}
	glEnd();
	glPopMatrix();
}