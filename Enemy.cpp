#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

Enemy::Enemy(int x, int y)
{
	x_pos = x;
	y_pos = y;

	active = true;
	speed = 5;


	boxright = 60;
	boxleft= 10;
	boxheight = 15;
}

void Enemy::set(int x, int y)
{
	x_pos = x;
	y_pos = y;

	active = true;
	speed = 3;
	boxright = 60;
	boxleft = 10;
	boxheight = 15;
}

void Enemy::Move(bool check)
{
	if (check == true)
	{
		speed *= -1;
	}
	 x_pos += speed;
}
