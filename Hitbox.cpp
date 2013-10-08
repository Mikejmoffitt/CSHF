#include "Hitbox.h"

Hitbox::Hitbox()
{
	vulnImg = al_load_bitmap("./data/hud/vuln.png");
	atckImg = al_load_bitmap("./data/hud/atck.png");
	bodyImg = al_load_bitmap("./data/hud/body.png");
}

void Hitbox::addFrame(int ecks, int why, int widf, int hite)
{
	xPos.push_back(ecks);
	yPos.push_back(why);
	width.push_back(widf);
	height.push_back(hite); 
	disabled.push_back(false);
}