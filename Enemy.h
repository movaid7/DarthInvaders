#pragma once

using namespace std;
#include "Global.h"	

class Enemy
{
public:
	Enemy();
	~Enemy();
	Enemy(int, int, int, int, int);
	void set(int, int, int, int, int);
	void Move();

	int x_pos;
	int y_pos;
	bool active;
	int speed;

	//movement bounds
	int rightbound;
	int leftbound;
	int ybound;

	//collision detection variables
	int boxright;
	int boxleft;
	int boxheight;
};

