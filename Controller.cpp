#include "Controller.h"

Controller::Controller()
{
	// Clear it out meng
	this->disabled = false;
	for (int i = 0; i < 4; i++)
	{
		this->stick[i] = 0;
	}
	for (int i = 0; i < 2; i++)
	{
		this->buttons[i] = 0;
	}
	for (int i = 0; i < 2; i++)
	{
		this->exbuttons[i] = 0;
	}
}

void Controller::setKeys(int up, int down, int left, int right, int a, int b, int start, int select)
{
	this->keys[0] = up;
	this->keys[1] = down;
	this->keys[2] = left;
	this->keys[3] = right;
	this->keys[4] = a;
	this->keys[5] = b;
	this->keys[6] = start;
	this->keys[7] = select;
}