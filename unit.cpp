#include "unit.h"

#define _USE_MATH_DEFINES

Unit::Unit(int _id, double *_path_x, double *_path_y, int _nodes){
	this->path_x = _path_x;
	this->path_y = _path_y;
	this->current_node = 2;
	this->nodes = _nodes;

	this->pos_x = *this->path_x;
	this->pos_y = *this->path_y;
	this->pos_z = 0.1;

	this->target_pos_x = *(this->path_x + 1);
	this->target_pos_y = *(this->path_y + 1);
}

Unit::~Unit(){
	free(this->path_x);
	free(this->path_y);
}

void Unit::update(){
    this->update_displacement();
    this->update_direction();
}

void Unit::draw(){
    glPushMatrix();
    glTranslatef(this->pos_x, this->pos_y, this->pos_z);
    glRotatef(this->direction * 180 / M_PI, 0, 0, 1);

    //Draw Base
	glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex3f(-this->radius, -this->radius, 0);
    glVertex3f(this->radius, -this->radius, 0);
    glVertex3f(this->radius, this->radius, 0);
    glVertex3f(-this->radius, this->radius, 0);

    //Draw Head
    glColor3f(1.0,0.1,0.1);
    glVertex3f(-this->radius/2, -this->radius/2, 0);
    glVertex3f(this->radius, -this->radius/2, 0);
    glVertex3f(this->radius, this->radius/2, 0);
    glVertex3f(-this->radius/2, this->radius/2, 0);
    glEnd();

    glPopMatrix();
}

void Unit::update_direction(){
    //Reset the bounds
    if(this->direction > 2*M_PI)
        this->direction -= 2*M_PI;
    if(this->direction < 0)
        this->direction += 2*M_PI;

    //Set target direction
    this->target_direction = atan2(this->target_pos_y - this->pos_y, this->target_pos_x - this->pos_x);

    if(this->target_direction < 0) //If it is negative change to between 180-360
        this->target_direction += 2*M_PI;

    //Rotate if needed
    if(this->target_direction != this->direction){
        if(almost_equal(this->target_direction, this->direction, this->rotation_speed)){
            this->direction = this->target_direction;
        }
        else{
            double length = this->target_direction - this->direction;
            if(length < 0){
                length *= -1;
                if(length < M_PI)
                    this->direction -= this->rotation_speed;
                else
                    this->direction += this->rotation_speed;
            }
            else{
                if(length < M_PI)
                    this->direction += this->rotation_speed;
                else
                    this->direction -= this->rotation_speed;
            }
        }
    }
}

void Unit::update_displacement(){
    //Check if needed to change
    if(almost_equal(this->target_pos_y, this->pos_y, this->MOVEMENT_EPSILON) &&
       almost_equal(this->target_pos_x, this->pos_x, this->MOVEMENT_EPSILON)){
		if (current_node == nodes){
			this->current_node = 1;
			this->target_pos_x = *(this->path_x + this->current_node - 1);
			this->target_pos_y = *(this->path_y + this->current_node - 1);
		}
		else{
			this->current_node++;
			this->target_pos_x = *(this->path_x + this->current_node - 1);
			this->target_pos_y = *(this->path_y + this->current_node - 1);
		}
    }

    //Update speed
    this->speed += this->acc;
    if(this->speed > this->max_speed)
        this->speed = this->max_speed;

    //Update position
    this->pos_x += this->speed*cos(this->direction);
    this->pos_y += this->speed*sin(this->direction);
}