#include "camera.h"

Camera::Camera(int x, int y){
	this->x = x;
	this->y = y;
	this->speed = 0.15;
}

Camera::~Camera(){

}

void Camera::update(int key_down[]){
	//Left
	if(key_down[0])
		this->x -= this->speed;
	//Right
	if(key_down[1])
		this->x += this->speed;
	//Up
	if(key_down[2])
		this->y += this->speed;
	//Down
	if(key_down[3])
		this->y -= this->speed;
}

void Camera::set(){
	gluLookAt(this->x, this->y, 6.0,
			  this->x, this->y, 0.0,
			  0.0, 1.0, 0.0);
}