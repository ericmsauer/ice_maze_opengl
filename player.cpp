#include "player.h"

#define _USE_MATH_DEFINES

Player::Player(){
	this->pos_x = 13.0;
	this->pos_y = 13.0;
    this->pos_z = 0.1;
    this->target_pos_x = this->pos_x;
    this->target_pos_y = this->pos_y;

	this->items = (int *) malloc(this->max_items * sizeof(int));
}

Player::~Player(){
	free(this->items);
}

void Player::death(double x, double y){
    set_pos(x,y);
}

void Player::update(){
    this->update_direction();
    this->update_displacement();
}

void Player::draw(){
    glPushMatrix();
    glTranslatef(this->pos_x, this->pos_y, this->pos_z);
    glRotatef(this->direction * 180 / M_PI, 0, 0, 1);

    //Draw Base
	glBegin(GL_QUADS);
    glColor3f(0.1, 0.2, 0.3);
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

    //Draw Direction
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(0, 0);
	glVertex2f(5,0);
	glEnd();

    glPopMatrix();
}

void Player::update_direction(){
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
    if(this->target_direction != this->direction && !this->reached_target_pos){
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

void Player::update_displacement(){
    //Check if needed to change
    if(almost_equal(this->target_pos_y, this->pos_y, this->MOVEMENT_EPSILON) &&
       almost_equal(this->target_pos_x, this->pos_x, this->MOVEMENT_EPSILON))
            this->reached_target_pos = true;

    //If on ice
    if(this->max_speed_on_ice){
        this->acc = 0.005;
        this->max_speed = this->max_speed_on_ice;
    }
    else{ //If on anything else
        //If it was on ice then change to ground
        if(this->max_speed != this->max_speed_on_ground){
            this->max_speed = this->max_speed_on_ground;
            this->reached_target_pos = true;
        }
        if(this->reached_target_pos){
            this->speed = 0.0;
            this->acc = 0.0;
        }
        else {
            this->acc = 0.005;
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

void Player::reset_displacement(){
    this->speed -= this->acc;
    this->pos_x -= this->speed*cos(this->direction);
    this->pos_y -= this->speed*sin(this->direction);
}

bool Player::give_item(int id){
	if (this->num_items == 5)
		return false;
	else
		this->items[this->num_items] = id;
	return true;
}

bool Player::remove_item(int id){
	for (int i = 0; i < this->num_items; i++){
		if (this->items[i] == id){
			this->items[i] = 0;
			this->num_items--;
			return true;
		}
	}
	return false;
}

bool Player::has_item(int id){
	for (int i = 0; i < this->num_items; i++){
		if (this->items[i] == id){
			return true;
		}
	}
	return false;
}
