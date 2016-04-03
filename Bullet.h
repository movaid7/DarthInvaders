#pragma once
#include "Spaceship.h"
#include "Global.h"	
#include "Enemy.h"

class Bullet {

public:
	int x_pos;
	int y_pos;
	int radius;
	int status;
	int speed;
	bool isFriendly;
	Bullet(int , int , int , bool );
	Bullet();
	~Bullet();
	void Increment();
	void UpdateBulletPos(Spaceship&);
	void Updatebulletpos(Enemy&);
};