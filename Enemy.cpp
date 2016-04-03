#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

Enemy::Enemy(int x, int y, int rb, int lb, int yb)
{
	x_pos = x;
	y_pos = y;
	rightbound = rb;
	leftbound = lb;
	ybound = yb;
	active = true;
	speed = 3;
	boxright = 60;
	boxleft= 10;
	boxheight = 15;
}

void Enemy::set(int x, int y, int rb, int lb, int yb)
{
	x_pos = x;
	y_pos = y;
	rightbound = rb;
	leftbound = lb;
	ybound = yb;
	active = true;
	speed = 3;
	boxright = 60;
	boxleft = 10;
	boxheight = 15;
}

void Enemy::Move()
{
	x_pos += speed;

	if (x_pos >= rightbound)
		speed *= -1;
	if (x_pos < leftbound)
		speed *= -1;
}