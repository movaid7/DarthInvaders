#include "Spaceship.h"

extern int width;
extern int height;

Spaceship::Spaceship(int x, int y)
{
	x_pos = x;
	y_pos = y;	
	boxright = 60;
	boxleft = 60;
	boxheight = 15;
	health = 60;
	active = true;
}

Spaceship::Spaceship() {}


void Spaceship::MoveSpaceshipLeft()//left bound
{
	if (x_pos >= 50)
		x_pos = x_pos - 7;
}

void Spaceship::MoveSpaceshipRight() //right bound
{
	if (x_pos<width - 50)
		x_pos = x_pos + 7;
}