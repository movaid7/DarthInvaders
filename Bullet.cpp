#include "Bullet.h"

Bullet::Bullet(int X, int Y,int S, bool friendly)
{
	x_pos = X;
	y_pos = Y;
	status = 0;
	speed = S;
	isFriendly = friendly;
}

Bullet::Bullet(){}

Bullet::~Bullet(){}

void Bullet::UpdateBulletPos(Spaceship &s)
{
	x_pos = s.x_pos;
	y_pos = s.y_pos;
}

void Bullet::Updatebulletpos(Enemy &e)
{
	x_pos = e.x_pos;
	y_pos = e.y_pos;
}

void Bullet::Increment()
{
	if (isFriendly)
		y_pos -= speed;
	else
		y_pos += speed;

	if (y_pos<10 || y_pos >height-10)				//if bullet reaches top or bottom
		status = 0;
}