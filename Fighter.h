#ifndef FIGHTER_H
#define FIGHTER_H
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "Sequence.h"
#include "Hit.h"
#include "Stats.h"
#include "Controller.h"
#include "Attack.h"

class Fighter
{
private:

	// Various variables used for checking for special commands
	int backCharge;
	int downCharge;

	//Animation sequences
	std::vector<Sequence> animations;

	// Attack sequences
	std::vector<Attack> attacks;

	// Player's name
	std::string name;

	bool playerTwo;

	// Pointer to the other player
	Fighter* opponent;

	// Stats
	Stats stats;

	
	// Whether the player is on the ground
	bool grounded;
	
	double health;

	// Position storage, and movement vectors
	double xPos;
	double yPos;
	double vecX;
	double vecY;
	double pushBackX;
	double projPushBackX;
	double slideX;

	// Whether or not the player is in a position to give input
	bool inputOk;

	// Whether or not the player is crouching
	bool crouching;

	// Hitfreeze to halt all action
	int hitFreeze; 

	// Where the ground is.
	const int GROUNDPOS;

	// Left and right movement boundaries.
	int minX;
	int maxX;

	// Hitstun reduces by one per frame if greater than zero
	int hitStun;

	// Blockstun acts like hitstun, but is during a blocking state
	int blockStun;

	// Delay before a jump; when it reaches 1 the character jumps.
	int jumpDel;

	// Delay after landing before input is returned.
	int landDel;

	// Whether an attack can occur
	bool attackOk;

	// Whether a special attack can occur (used for special cancels)
	bool specialOk;

	// If greater than zero, an attack is currently in progress
	int attackCnt;

	// Which attack is occuring
	int currentAttack;
	
	// Which animation is playing
	int currentAnim;

	// Whether or not to use regular VecX (or the half-push one)
	bool halfPush;

	// Processes the inputs to determine if it is time to do an attack
	// or any other thing based on input that isn't simple movement
	/*
	Basic structure of this function:
	check whether the player is holding forward or back
	if the player has pressed button 0 or 1 (A or B):
		check for queued special inputs (qcf, qcb, fdp, bdp, hcf, hcb)
		if queued inputs:
			do corrosponding attack
		otherwise, check for player state (air, crouching, close, standing)
		based on player state and stick state, doAttack(int a)
	*/
	void processInputs();

	// "Does" the numbered attack.
	void doAttack(int a);

public:
	// Which direction we are facing (0 is left)
	bool direction;

	// Some general sound effects
	ALLEGRO_SAMPLE *crouchSnd;
	ALLEGRO_SAMPLE *standSnd;
	ALLEGRO_SAMPLE *landSnd;
	ALLEGRO_SAMPLE *jumpSnd;
	
	// Checks if two boxes are overlapping
	bool boxCol(int x1, int x2, int y1, int y2, int x3, int x4, int y3, int y4);

	// Whether or not to blit hitboxes
	bool showBox;

	// Dimensions for the animation frames
	const int HEIGHT;
	const int WIDTH;

	// Input
	Controller input;

	// Scale value for shadow
	float shadowHeight;

	// Loads a character from files
	void loadFighter();

	// Chooses the correct animation
	void calcAnimation();

	// Converts a string to a double
	double stringToDouble( const std::string& s );

	// Instantiation
	Fighter(double xPos, double yPos, std::string nombre, bool p2, int w, int h);
	
	// Returns position
	double getX();
	double getY();

	// Process crouching
	void processCrouch();

	// Processes jumping
	void processJumps();
	
	// Sets position
	void setPos(double x, double y);
	
	// Pushbacks
	void pushBacks();

	// Checks for bounds
	void boundChecks();

	// Processes attacks in progress
	void processAttacks();

	// Main physics routine
	void doPhysics();

	// Blit routine
	void blit(int scroll);

	// Adds a sequence
	void addSequence(Sequence addMe);

	// Sets animation
	void setAnim(int which);

	// Sets animation and forces frame 0 at start
	void forceAnim(int which);

	// Sets direction
	void setDir(bool dir);

	// Player walking
	void walkTheWalk();

	// Allows definition of opponent pointer
	void setOpponent(Fighter* other);

	// Figures out the direction the player should be facing
	void calcDirection();

	// Sets the minimum and maximum X
	void calcExtremes(int scroll);

	// Checks for whether or not the screen should be scrolling
	int pushingBounds();

	// Returns VecX
	double getVecX();

};

#endif