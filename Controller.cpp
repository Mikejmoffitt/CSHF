#include "Controller.h"

Controller::Controller()
{
	// Clear it out meng
	this->disabled = false;
	for (int i = 0; i < 4; i++)
	{
		stick[i] = 0;
	}
	for (int i = 0; i < 2; i++)
	{
		buttons[i] = 0;
	}
	for (int i = 0; i < 2; i++)
	{
		exbuttons[i] = 0;
	}
}

void Controller::setKeys(int up, int down, int left, int right, int a, int b, int start, int select)
{
	keys[0] = up;
	keys[1] = down;
	keys[2] = left;
	keys[3] = right;
	keys[4] = a;
	keys[5] = b;
	keys[6] = start;
	keys[7] = select;
}