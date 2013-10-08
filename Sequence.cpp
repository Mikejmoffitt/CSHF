#include "Sequence.h"
#include <string>

Sequence::Sequence()
{
	animFrame = 0;
	animCnt = 0;
}
void Sequence::setLoopBack(int loop)
{
	loopBack = loop;
}
unsigned int Sequence::numFrames()
{
	return frames.size();
}
void Sequence::loadFrame(std::string fName, int plength, int plxoff, int plyoff, bool pflip)
{
	addFrame(al_load_bitmap(fName.c_str()),plength,plxoff,plyoff,pflip);
}
void Sequence::addFrame(ALLEGRO_BITMAP* pframe, int plength, int plxoff, int plyoff, bool pflip)
{
	frames.push_back(pframe);
	lengths.push_back(plength);
	xoff.push_back(plxoff);
	yoff.push_back(plyoff);
	flip.push_back(pflip);
	frameNum = frames.size();
}
ALLEGRO_BITMAP* Sequence::getFrame() // Return the current animation frame for rendering
{
	return frames[animFrame];
}
ALLEGRO_BITMAP* Sequence::getFrame(int which) // Return a specific frame
{
	return frames.at(which);
}
void Sequence::animate() // This runs once per tick
{
	if (frames.size()-1 > 0)
	{
		if (animCnt == lengths[animFrame]) // If the frame is finished being displayed...
		{
			animCnt = 0; // Reset display counter
			if (frameNum > 0)
			{
				animFrame = animFrame + 1; // increment frame number
			}
		}
		if (animFrame > frames.size()-1) // If the animation has completed its loop...
		{
			animFrame = loopBack; // Loop back to the designated frame (often 0)
			animCnt = 0; // Reset display counter
		}
		animCnt = animCnt + 1; // increment display counter
	}
	else
	{
		animFrame = 0;
		animCnt = 0;
	}
}

void Sequence::blitShadow(int xpos, int ypos, bool flip, float mult)
{
	int theHeight = al_get_bitmap_height(this->getFrame());
	int theX = xpos+(this->xoff[this->animFrame]);
	int theY = ypos-(this->yoff[this->animFrame])+(int((1.0 - mult) * (theHeight)));
	if (flip ^ this->flip[this->animFrame])
	{
		al_draw_tinted_scaled_bitmap(this->getFrame(),al_map_rgb(0,0,0),0,0,al_get_bitmap_width(this->getFrame()),theHeight,theX,theY,al_get_bitmap_width(this->getFrame()),theHeight*mult,ALLEGRO_FLIP_HORIZONTAL);
	}
	else
	{
		al_draw_tinted_scaled_bitmap(this->getFrame(),al_map_rgb(0,0,0),0,0,al_get_bitmap_width(this->getFrame()),theHeight,theX,theY,al_get_bitmap_width(this->getFrame()),theHeight*mult,0);
	}
}

void Sequence::blit(int xpos, int ypos, bool flip)
{
	ALLEGRO_COLOR *tint = 0;
	// Blits to the screen.
	if (flip ^ this->flip[this->animFrame])
	{
		al_draw_bitmap(this->getFrame(),xpos+(this->xoff[this->animFrame]),ypos+(this->yoff[this->animFrame]),ALLEGRO_FLIP_HORIZONTAL);
	}
	else
	{
		al_draw_bitmap(this->getFrame(),xpos+(this->xoff[this->animFrame]),ypos+(this->yoff[this->animFrame]),0);
	}
}

std::vector<int> Sequence::getRealBoxDim(Hitbox box, int xpos, int ypos, bool direction)
{
	int theX;
	if (direction ^ flip[this->animFrame])
	{
		theX = xpos + (al_get_bitmap_width(this->getFrame()) - box.xPos[this->animFrame] - box.width[this->animFrame]);
	}
	else
	{
		theX = xpos + box.xPos[this->animFrame];
	}
	int theY = ypos + box.yPos[this->animFrame];
	int theX2 = theX + box.width[this->animFrame];
	int theY2 = theY + box.height[this->animFrame];
	theX = theX + this->xoff[this->animFrame];
	theY = theY + this->yoff[this->animFrame];
	theX2 = theX2 + this->xoff[this->animFrame];
	theY2 = theY2 + this->yoff[this->animFrame];
	std::vector<int> returns;
	returns.push_back(theX);
	returns.push_back(theX2);
	returns.push_back(theY);
	returns.push_back(theY2);
	return returns;
}

void Sequence::drawHitBox(Hitbox box, int xpos, int ypos, bool direction, int type)
{
	int theX;
	if (direction ^ flip[this->animFrame])
	{
		theX = xpos + (al_get_bitmap_width(this->getFrame()) - box.xPos[this->animFrame] - box.width[this->animFrame]);
	}
	else
	{
		theX = xpos + box.xPos[this->animFrame];
	}
	theX = theX + this->xoff[this->animFrame];
	int theY = ypos + box.yPos[this->animFrame];
	theY = theY + this->yoff[this->animFrame];
	int width = box.width[this->animFrame];
	int height = box.height[this->animFrame];
	if (type == 0)
	{
		al_draw_scaled_bitmap(box.vulnImg,0,0,1,1,theX,theY,width,height,0);
	}
	else if (type == 1)
	{
		al_draw_scaled_bitmap(box.atckImg,0,0,1,1,theX,theY,width,height,0);
	}
	else
	{
		al_draw_scaled_bitmap(box.bodyImg,0,0,1,1,theX,theY,width,height,0);
	}
}

void Sequence::drawBoxes(int xpos, int ypos, bool direction)
{
	drawHitBox(hitBox,xpos,ypos,direction,0);
	drawHitBox(hitBox1,xpos,ypos,direction,0);
	drawHitBox(hitBox2,xpos,ypos,direction,0);
	drawHitBox(atkBox,xpos,ypos,direction,1);
	drawHitBox(bodyBox,xpos,ypos,direction,2);
}

int Sequence::getAnimationLength()
{
	unsigned int index = 0;
	int counter = 0;
	while (index < frames.size())
	{
		counter = counter + lengths[index];
		index = index + 1;
	}
	return counter;
	return 0;
}
void Sequence::reset()
{
	animFrame = 0;
	animCnt = 0;
}
void Sequence::reset(int theFrame)
{
	animFrame = theFrame;
	animCnt = 0;
}