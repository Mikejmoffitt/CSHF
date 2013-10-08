#ifndef HITBOX_H
#define HITBOX_H

#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

struct Hitbox
{
public:
	ALLEGRO_BITMAP* vulnImg;
	ALLEGRO_BITMAP* atckImg;
	ALLEGRO_BITMAP* bodyImg;
	Hitbox();
	void addFrame(int ecks, int why, int widf, int hite);
	std::vector<int> xPos;
	std::vector<int> yPos;
	std::vector<int> width;
	std::vector<int> height;
	std::vector<bool> disabled;
};

#endif