#ifndef ATTACK_H
#define ATTACK_H

#include <vector>
#include "Sequence.h"
#include "Hit.h"
struct Attack
{
	Attack();

public:
	int animNum;
	int damage;
	int hitStun;
	int hitFreeze;
	int whifSnd;
	int hitSnd;
	int hits;
	int holdFrames;
	int holdBtn;

	double vecX;
	double vecY;
	double dVecX;
	double dVecY;

	double recoilVecX;
	double recoilVecY;
	bool groundLaunch;

	int shiftX;
	bool knockDown;

};
#endif