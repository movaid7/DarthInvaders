#include "Spaceship.h"


Spaceship::Spaceship(int x, int y)
{
	x_pos = x;
	y_pos = y;	
	boxright = 60;
	boxleft = 60;
	boxheight = 15;
	health = 100;
}

Spaceship::Spaceship() {}

void Spaceship::MoveSpaceshipUp()	//won't need
{
	if (y_pos >= 10)
		y_pos = y_pos - 7;
}

void Spaceship::MoveSpaceshipDown()	//won't need
{
	if (y_pos<height - 200)
		y_pos = y_pos + 7;
}

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