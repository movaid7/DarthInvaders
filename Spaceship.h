#pragma once

#include<allegro5\allegro.h>
#include<allegro5\allegro_primitives.h>
#include "Global.h"	

class Spaceship
{
public:
	int x_pos;
	int y_pos;
	int boxright;
	int boxleft;
	int boxheight;
	int health;
	bool active;

	Spaceship(int, int);
	Spaceship();

	void MoveSpaceshipLeft();

	void MoveSpaceshipRight();
};