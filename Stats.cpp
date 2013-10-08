#include "Stats.h"
Stats::Stats(){};

void Stats::loadStats(std::vector<double> data)
{
	FWDSPEED = data.at(0);
	BCKSPEED = data.at(1);
	JFWDSPEED = data.at(2);
	JBCKSPEED = data.at(3);
	JSTRENGTH = data.at(4);
	GRAVITY = data.at(5);
	FRICTION = data.at(6);
	JUMPDELAY = int(data.at(7));
	LANDDELAY = int(data.at(8));
	FLIPSPR = int(data.at(9));
	WIDTH = int(data.at(10));
	HEIGHT = int(data.at(11));
};