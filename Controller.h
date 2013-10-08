#ifndef CONTROLLER_H
#define CONTROLLER_H

struct Controller
{
public:
	int stick[4];
	int buttons[2];
	int exbuttons[2];
	bool disabled;
	int keys[8];
	Controller();
	void setKeys(int up, int down, int left, int right, int a, int b, int start, int select);
};

#endif
