#include "Fighter.h"
/*
Animations:
0 - Standing
1 - Walking Forward
2 - Walking backward
3 - Crouching
4 - Blocking (high)
5 - Blocking (low)
6 - Jumping up
7 - Jumping forward
8 - Jumping back
9 - Hurt in air
10 - Hurt high
11 - Hurt medium
12 - Hurt low
13 - Lying on ground
14 - Getting up
15 - Grabbing
16 - Being Grabbed
17 - Throwing
18 - Being Thrown
19 - KO'd
20 - Victory

// Normal attack animations
21 - Standing 1
22 - Standing 2
23 - Standing 3
24 - Standing 4
25 - Standing 5
26 - Standing 6

27 - Crouching 1
28 - Crouching 2
29 - Crouching 3
30 - Crouching 4
31 - Crouching 5
32 - Crouching 6

33 - Air 1
34 - Air 2
35 - Air 3
36 - Air 4
37 - Air 5
38 - Air 6

Character's Origin:
 _______________
|               |
|               |
|               |
|               |
|_______o_______|

*/


#define SHOWINGBOXES false

Fighter::Fighter(double x, double y, std::string nombre, bool p2, int w, int h) : GROUNDPOS(220), WIDTH(w), HEIGHT(h)
{
	playerTwo = p2;
	inputOk = true;
	crouching = false;
	name = nombre;
	vecX = 0;
	vecY = 0;
	xPos = x;
	yPos = y;
	minX = 20;
	maxX = 300;
	pushBackX = 0;
	projPushBackX = 0;
	slideX = 0;
	hitStun = 0;
	currentAnim = 0;
	jumpDel = 0;
	landDel = 0;
	attackCnt = 0;
	blockStun = 0;
	hitFreeze = 0;
	loadFighter();
	shadowHeight = -0.13;
	showBox = SHOWINGBOXES;
}

bool Fighter::boxCol(int x1, int x2, int y1, int y2, int x3, int x4, int y3, int y4)
{
	// Checks if the max boundaries of any boxes are 
	// exceeding any min boundaries of the other box
	return !(x2 < x3 || x1 > x4 || y2 < y3 || y1 > y4 );
}

double Fighter::getX()
{
	return xPos;
}

double Fighter::getY()
{
	return yPos;
}
double Fighter::stringToDouble( const std::string& s ) 
{
	std::istringstream i(s);
	double x;
	if (!(i >> x))
		return 0;
	return x;
} 

std::vector<std::string> tokenize(std::string s, char delim)
{
	const char EOL = '\n';
	std::vector<std::string> elements;
	std::string elem;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == delim || s[i] == EOL)
		{
			elements.push_back(elem);
			elem = "";
		}
		else
		{
			elem.push_back(s[i]);
		}
	}
	return elements;
}

void Fighter::loadFighter()
{
	// Load some basic samples
	crouchSnd = al_load_sample(("./data/char/" + this->name + "/sfx/crouch.ogg").c_str());
	standSnd = al_load_sample(("./data/char/" + this->name + "/sfx/stand.ogg").c_str());
	landSnd = al_load_sample(("./data/char/" + this->name + "/sfx/land.ogg").c_str());
	jumpSnd = al_load_sample(("./data/char/" + this->name + "/sfx/jump.ogg").c_str());

	// Load stats
	std::ifstream loadStat;
	loadStat.open ("./data/char/" + this->name + "/stats.txt", std::ifstream::in);
	std::vector<double> datums;
	std::string pushMe;
	while (loadStat.good())
	{
		getline(loadStat,pushMe);
		datums.push_back(stringToDouble(pushMe));
	}
	stats.loadStats(datums);

	// Load the basic animations for the player
	// This does not include special attack animations yet
	std::cout << "Loading animations for " << name << std::endl;
	for (int i = 0; i <= 26; i++) // For every animation txt in the anim directory... TODO have it figure this out, instead of some stupid hardcoded 26 or #define
	{
		std::stringstream file;
		Sequence addSeq;
		std::string fName;
		std::string boxDim;
		std::string loopMetric;
		std::string inString;

		int frameDur = 0;
		int xOff = 0;
		int yOff = 0;
		if (i <= 20)
		{
			file << "./data/char/" << this->name << "/anim/norm/" << i << ".txt";
		}
		else
		{
			file << "./data/char/" << this->name << "/anim/atk/" << i << ".txt";
		}
		loadStat.close();
		loadStat.open(file.str());
		/* IntCnt:
		0 is the filename.
		1 is length of frame
		2 is xoff
		3 is yoff
		4 is hitboxes onwards...
		*/
		int count = 0;
		std::vector<Sequence> addSeqs;
		addSeqs.push_back(Sequence());
		while (loadStat.good())
		{
			fName = "";
			inString = "";
			boxDim = "";
			int frameDur = 0;
			int xOff = 0;
			int yOff = 0;
			int loopMetric = 0;
			getline(loadStat,inString);
			if (inString[0] == '~') // Marker for end of animation
			{
				loopMetric = atoi(inString.substr(1,9).c_str());
				addSeqs.back().setLoopBack(loopMetric);
			}
			else // Add an animation frame
			{
				// Split the animation line into multiple elements
				std::vector<std::string> elements = tokenize(inString, ',');
				// Filename for the frame bitmap itself
				fName = elements[0];
				// Duration of the frame as an int
				frameDur = int(stringToDouble(elements[1]));
				// X offset
				xOff = int(stringToDouble(elements[2]));
				// Y offset
				yOff = int(stringToDouble(elements[3]));
				// Box 0 coords (relative) and width/height
				// Adds to hitBox, which is added to addSeq.
				addSeqs.back().hitBox.addFrame(atoi(elements[4].c_str()),
					atoi(elements[5].c_str()),
					atoi(elements[6].c_str()),
					atoi(elements[7].c_str()));
				addSeqs.back().hitBox1.addFrame(atoi(elements[8].c_str()),
					atoi(elements[9].c_str()),
					atoi(elements[10].c_str()),
					atoi(elements[11].c_str()));
				addSeqs.back().hitBox2.addFrame(atoi(elements[12].c_str()),
					atoi(elements[13].c_str()),
					atoi(elements[14].c_str()),
					atoi(elements[15].c_str()));
				addSeqs.back().atkBox.addFrame(atoi(elements[16].c_str()),
					atoi(elements[17].c_str()),
					atoi(elements[18].c_str()),
					atoi(elements[19].c_str()));
				addSeqs.back().bodyBox.addFrame(atoi(elements[20].c_str()),
					atoi(elements[21].c_str()),
					atoi(elements[22].c_str()),
					atoi(elements[23].c_str()));

				// Finally, addSeqs has the frame bitmap, earlier variables, 
				// and sprite flip boolean added
				addSeqs.back().loadFrame("data/char/" + name + "/sprites/" + 
					fName + ".png",frameDur,xOff,yOff,stats.FLIPSPR);
				}
		}
		// Once the sequence has been built, the sequence itself 
		// is added to the player's vector of sequences
		addSequence(addSeqs.back());
	}
}
	
void Fighter::setPos(double x, double y)
{
	xPos = x;
	yPos = y;
}

void Fighter::blit(int scroll)
{
	int xPut = int(xPos-(WIDTH/2)+scroll);
	int yPut = int(yPos-HEIGHT);
	Sequence* displayMe;
	//if (attackCnt == 0)
		displayMe = &animations[currentAnim];
	//else
	//{
		// displayMe = &attacks[currentAttack].animation;
	//}
	displayMe->blitShadow(xPut, (GROUNDPOS-HEIGHT)+((GROUNDPOS-yPos)*shadowHeight*-1) - 2, direction, shadowHeight);
	displayMe->blit(xPut, yPut, direction);
	if (showBox)
	{
		displayMe->drawBoxes(xPut, yPut, direction);
	}
}

void Fighter::setOpponent(Fighter* other)
{
	opponent = other;
}

void Fighter::processJumps()
{
	if (attackCnt > 0)
	{
		jumpDel = 0;
		landDel = 0;
	}
	if (grounded && attackCnt == 0)
	{
		if (landDel == stats.LANDDELAY)
		{
			al_play_sample(landSnd, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		if (inputOk && input.stick[0] > 0 && !crouching)
		{
			jumpDel = stats.JUMPDELAY;
		}
		if (jumpDel == stats.JUMPDELAY)
		{
			al_play_sample(crouchSnd, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		if (landDel > 0 && jumpDel == 0)
		{
			landDel = landDel - 1;
			vecY = 0;
			vecX = 0;
		}
		if (jumpDel > 1)
		{
			vecY = 0;
			vecX = 0;
			jumpDel = jumpDel - 1;
		}
		else if (jumpDel == 1)
		{
			al_play_sample(jumpSnd, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			jumpDel = 0;
			if (input.stick[2] > 0 && input.stick[3] == 0) // Player is holding left on joystick
			{
				if (direction == false)
				{
					vecX = stats.JFWDSPEED * -1;
				}
				else
				{
					vecX = stats.JBCKSPEED * -1;
				}
			}
			else if (input.stick[3] > 0 && input.stick[2] == 0) // Player is holding left on joystick
			{
				if (direction == true)
				{
					vecX = stats.JFWDSPEED;
				}
				else
				{
					vecX = stats.JBCKSPEED;
				}
			}
			else // No left/right inputs or both are being held (shouldn't be possible anyway)
			{
				vecX = 0;
			}
			vecY = stats.JSTRENGTH * -1;
			landDel = stats.LANDDELAY;
		}
	}
}

void Fighter::pushBacks()
{	
	// Pushes a player out of the other's bounding box
	halfPush = false;
	int x1,x2,x3,x4,y1,y2,y3,y4;
	std::vector<int> myBoxData = this->animations[currentAnim].getRealBoxDim(this->animations[currentAnim].bodyBox,xPos,yPos,direction);
	std::vector<int> otherBoxData = opponent->animations[currentAnim].getRealBoxDim(opponent->animations[opponent->currentAnim].bodyBox,opponent->xPos,opponent->yPos,opponent->direction);
	x1 = myBoxData[0];
	x2 = myBoxData[1];
	y1 = myBoxData[2];
	y2 = myBoxData[3];
	x3 = otherBoxData[0];
	x4 = otherBoxData[1];
	y3 = otherBoxData[2];
	y4 = otherBoxData[3];
	if (boxCol(x1,x2,y1,y2,x3,x4,y3,y4))
	{
		if (xPos < opponent->xPos)
		{
			xPos = xPos - 1;
			opponent->xPos = opponent->xPos + 1;
		}
		else
		{
			xPos = xPos + 1;
			opponent->xPos = opponent->xPos - 1;
		}
	}

	if (boxCol(x1,x2,y1,y2,x3,x4,y3,y4))
	{
		if ((x2 >= x3 && vecX > 0 && xPos < opponent->xPos)
			|| (x1 <= x4 && vecX < 0 && xPos > opponent->xPos))
		{
			halfPush = true;
			opponent->xPos = opponent->xPos + (vecX / 2);
		}
	}
	while (boxCol(x1,x2,y1,y2,x3,x4,y3,y4))
	{
		halfPush = true;
		std::vector<int> myBoxData = this->animations[currentAnim].getRealBoxDim( this->animations[currentAnim].bodyBox , xPos , yPos , direction );
		std::vector<int> otherBoxData = opponent->animations[currentAnim].getRealBoxDim( opponent->animations[opponent->currentAnim].bodyBox , opponent->xPos , opponent->yPos , opponent->direction );
		x1 = myBoxData[0];
		x2 = myBoxData[1];
		x3 = otherBoxData[0];
		x4 = otherBoxData[1];
		if (xPos < opponent->xPos)
		{
			xPos--;
		}
		else
		{
			xPos++;
		}
	}
	if (opponent != NULL)
	{
		if ((pushBackX < 0 && xPos <= minX) || (pushBackX > 0 && xPos >= maxX))
		{
			opponent->xPos = opponent->xPos - pushBackX;
		}
		else
		{
			this->xPos = this->xPos + pushBackX;
		}
	}
	if (!((projPushBackX < 0 && xPos <= minX) || (projPushBackX > 0 && xPos >= maxX)))
	{
		xPos = xPos + projPushBackX;
	}
	if (pushBackX < 0)
	{
		pushBackX = pushBackX + stats.FRICTION;
	}
	else if (pushBackX > 0)
	{
		pushBackX = pushBackX - stats.FRICTION;
	}
	if ((pushBackX > 0 && pushBackX < stats.FRICTION)||(pushBackX < 0 && pushBackX > stats.FRICTION * -1))
	{
		pushBackX = 0;
	}
	if (projPushBackX < 0)
	{
		projPushBackX = projPushBackX + stats.FRICTION;
	}
	else if (projPushBackX > 0)
	{
		projPushBackX = projPushBackX - stats.FRICTION;
	}
	if ((projPushBackX > 0 && projPushBackX < stats.FRICTION)||(projPushBackX < 0 && projPushBackX > stats.FRICTION * -1))
	{
		projPushBackX = 0;
	}
}

void Fighter::calcDirection()
{
	if (inputOk)
	{
		if (opponent->xPos < this->xPos)
		{
			direction = false;
		}
		else
		{
			direction = true;
		}
	}
}

void Fighter::boundChecks()
{
	if (xPos > maxX)
	{
		xPos = maxX;
	}
	if (xPos < minX)
	{
		xPos = minX;
	}
}

void Fighter::processCrouch()
{
	if (input.stick[1] > 0 && inputOk)
	{
		if (crouching == false)
		{
			al_play_sample(crouchSnd, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		crouching = true;
	}
	else
	{
		if (crouching == true)
		{
			al_play_sample(standSnd, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		crouching = false;
	}
	if (crouching)
	{
		vecX = 0;
	}
}
/*
void walkTheWalk()
Moves the player forwards or backwards. 
*/
void Fighter::walkTheWalk()
{
	if (inputOk && !crouching && attackCnt == 0)
	{
		if (!direction)
		{
			if (input.stick[2] > 0)
			{
				vecX = stats.FWDSPEED * -1;
			}
			else if (input.stick[3] > 0)
			{
				vecX = stats.BCKSPEED;
			}
		}
		else
		{
			if (input.stick[2] > 0)
			{
				vecX = stats.BCKSPEED * -1;
			}
			else if (input.stick[3] > 0)
			{
				vecX = stats.FWDSPEED;
			}
		}
		if (!((input.stick[2] == 0) ^ (input.stick[3] == 0)))
		{
			vecX = 0;
		}
	}
}

/*
void calcAnimation()
Determines which animation to play during normal circumstances
(not attacking, or being hit, etc)
*/
void Fighter::calcAnimation()
{
	if (attackCnt == 0 && this->hitStun == 0)
	{
		if (grounded && (jumpDel > 0 || landDel > 0))
		{
			// Crouching before/after a jump
			setAnim(3);
		}
		else if (hitStun == 0)
		{
			if (grounded && inputOk)
			{
				if (vecX == 0)
				{
					if (crouching)
					{
						// Crouching
						setAnim(3);
					}
					else
					{
						// Standing
						setAnim(0);
					}
				}
				else
				{
					if ((vecX > 0) ^ (!direction))
					{
						// Advancing
						setAnim(1);
					}
					else
					{
						// Retreating
						setAnim(2);
					}
				}
			}
			else if (!grounded)
			{
				if (vecX == 0)
				{
					setAnim(6);
				}
				else
				{
					if (direction && vecX > 0 || !direction && vecX < 0)
					{
						setAnim(7);
					}
					else if (direction && vecX < 0 || !direction && vecX > 0)
					{
						setAnim(8);
					}
				}
			}
		}
	}
}

void Fighter::processInputs()
{
	// Used for holding "forward" (1) or "backward" (2) or none (0)
	int stickPosX = 0;
	if (input.stick[2] && !input.stick[3])
	{
		stickPosX = 1;
	}
	if (input.stick[3] && !input.stick[2])
	{
		stickPosX = 2;
	}
	if (!direction)
	{
		if (stickPosX == 1)
		{
			stickPosX = 2;
		}
		else if (stickPosX == 2)
		{
			stickPosX = 1;
		}
	}

	// Does normal attacks, if no specials are queued

	if (attackCnt == 0)
	{
		if (input.buttons[0] == 1) // Prioritize A button
		{
			if (stickPosX == 0)
			{
				doAttack(0);
			}
			else if (stickPosX == 1)
			{
				doAttack(1);
			}
			else
			{
				doAttack(2);
			}
		}
		else if (input.buttons[1] == 1) // Check B button
		{
			if (stickPosX == 0)
			{
				doAttack(3);
			}
			else if (stickPosX == 1)
			{
				doAttack(4);
			}
			else
			{
				doAttack(5);
			}
		}
	}
}

void Fighter::doAttack(int a)
{
	if (landDel == 0 && attackCnt == 0)
	{
		this->vecX = 0;
		this->currentAttack = a + (6 * this->crouching) + (12 * (!this->grounded));
		// Set that animation!
		this->forceAnim(currentAttack + 21);
		this->attackCnt = this->animations[currentAnim].getAnimationLength();
		std::cout << name << ": Doing attack #" << this->currentAttack << std::endl;
		attackOk = false;
	}
}

void Fighter::processAttacks()
{
	if (attackCnt > 0)
	{
		attackOk = false;
		attackCnt -= 1;
	}
}

void Fighter::doPhysics()
{
	if (this->hitFreeze > 0)
	{
		hitFreeze = hitFreeze - 1;
	}
	else
	{
		calcDirection();
		walkTheWalk();
		if (grounded && jumpDel == 0 && landDel == 0 && hitStun == 0 && blockStun == 0 && attackCnt == 0)
		{
			// This means the player is in a position to process inputs in general
			if (inputOk == false)
			{
				std::cout << name << ": Input ok!" << std::endl;
				if (attackCnt == 0)
				{
					attackOk = true;
					specialOk = true;
				}
			}
			inputOk = true;
		}
		else
		{
			if (inputOk == true)
			{
				std::cout << name << ": No input..." << std::endl;
			}
			inputOk = false;
			attackOk = false;
			specialOk = false;
		}	
		processInputs();
		processCrouch();
		processAttacks();
		grounded = (yPos >= GROUNDPOS);
		if (hitStun == 0)
		{
			processJumps();
			yPos = yPos + vecY;
			if (!halfPush)
			{
				xPos = xPos + vecX;
			}
			else
			{
				this->xPos = this->xPos + (vecX / 2);
			}
			pushBacks();
			boundChecks();
			if (yPos < GROUNDPOS)
			{
				vecY = vecY + stats.GRAVITY;
			}
			else if (yPos >= GROUNDPOS && vecY > 0)
			{
				yPos = GROUNDPOS;
				vecY = 0;
			}
		}
		calcAnimation();
		this->animations[currentAnim].animate();
	}
}

void Fighter::addSequence(Sequence addMe)
{
	this->animations.push_back(addMe);
}

void Fighter::setAnim(int which)
{
	if (which != currentAnim)
	{
		this->animations[currentAnim].reset();
	}
	this->currentAnim = which;
}

void Fighter::forceAnim(int which)
{
	this->animations[currentAnim].reset();
	this->currentAnim = which;
	this->animations[currentAnim].reset();
}

void Fighter::setDir(bool dir)
{
	this->direction = dir;
}

void Fighter::calcExtremes(int scroll)
{
	minX = 20-scroll;
	maxX = 300-scroll;
}

int Fighter::pushingBounds()
{
	if (xPos == minX && vecX < 0)
	{
		return 1;
	}
	else if (xPos == maxX && vecX > 0)
	{
		return 2;
	}
	return 0;
}

double Fighter::getVecX()
{
	return vecX;
}