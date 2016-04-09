#pragma once
#include "Global.h"

using namespace std;


class Enemy
{
public:
	Enemy();
	~Enemy();
	Enemy(int, int);
	void set(int, int);
	void Move(bool);

	int x_pos;
	int y_pos;

	bool active;
	int speed;

	//collision detection variables
	int boxright;
	int boxleft;
	int boxheight;
};

