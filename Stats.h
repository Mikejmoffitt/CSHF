#ifndef STATS_H
#define STATS_H
#include <vector>
#include <iostream>
struct Stats
{
	// [ Physics ]
	// dX for walking forwards
	double FWDSPEED;

	// dX for walking backwards
	double BCKSPEED;

	// dX for jumping forwards
	double JFWDSPEED;
	
	// dX for jumping backwards
	double JBCKSPEED;

	// Jump Strength 
	double JSTRENGTH;

	// Gravity coefficient
	double GRAVITY;

	// Sliding friction coefficient
	double FRICTION;

	// Frames before jump delay
	int JUMPDELAY;

	// Frames of recovery from jumping
	int LANDDELAY;

	// Whether or not to flip the sprites of the character
	int FLIPSPR;

	// Dimensions of sprites
	int WIDTH;
	int HEIGHT;

public:
	// constructor that takes no parameters
	Stats();

	// Loads stats from a passed vector
	void loadStats(std::vector<double> data);
};

#endif