#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <vector>
#include "Hitbox.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class Sequence
{

public:
	Hitbox hitBox; // Hitboxes corrosponding to the bitmaps
	Hitbox hitBox1; // Hitboxes corrosponding to the bitmaps
	Hitbox hitBox2; // Hitboxes corrosponding to the bitmaps
	Hitbox atkBox; // Hitboxes corrosponding to the bitmaps
	Hitbox bodyBox; // Hitboxes corrosponding to the bitmaps
	/*
	Default constructor.
	*/
	Sequence();
	/*
	Returns how many frames are in the sequence.
	*/
	unsigned int numFrames();
	/*
	Sets the loopback.
	*/
	void setLoopBack(int loop);
	/*
	Returns real-world coordinates for the frame's box
	Format:
	x1,x2,y1,y2
	*/	
	std::vector<int> getRealBoxDim(Hitbox box, int xpos, int ypos, bool direction);
	/*
	Loads a frame from a PNG file.
	*/
	void loadFrame(std::string fName, int plength, int plxoff, int plyoff, bool pflip);
	/*
	Adds on a new frame to the animation sequence.
	*/
	void addFrame(ALLEGRO_BITMAP* pframe, int plength, int plxoff, int plyoff, bool pflip);
	/* 
	Returns the current frame to be displayed.
	*/
	ALLEGRO_BITMAP* getFrame();
	/*
	Returns the requested frame. Uses at(n) so it is safe.
	*/
	ALLEGRO_BITMAP* getFrame(int which);
	/*
	Runs the animation script. Should occur once per frame.
	*/
	void animate();
	/*
	Gets the length of the animation in frames * lengths
	*/
	int getAnimationLength();

	void blit(int xpos, int ypos, bool flip);
	void blitShadow(int xpos, int ypos, bool flip, float mult);
	/*
	Resets the animation back to frame 0.
	*/
	void reset();
	/*
	Sets the animation frame to the passed argument.
	*/
	void reset(int frame);
	/*
	Draws hitboxes
	*/
	void drawBoxes(int xpos, int ypos, bool direction);
	void drawHitBox(Hitbox box, int xpos, int ypos, bool direction, int type);

	std::vector<ALLEGRO_BITMAP*> frames; // The bitmaps themselves

private:
	std::vector<int> lengths; // Corrosponding frame lengths
	std::vector<int> xoff; // X offset
	std::vector<int> yoff; // Y offset
	std::vector<bool> flip; // Whether or not to flip the frame horizontally
	
	unsigned int animFrame;
	unsigned int animCnt;
	unsigned int frameNum;
	unsigned int loopBack;

	Hitbox vuln1;
	Hitbox vuln2;
	Hitbox vuln3;
	Hitbox atck;
	Hitbox body;
};

#endif