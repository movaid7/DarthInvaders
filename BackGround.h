#pragma once

#include<allegro5\allegro.h>
#include<allegro5\allegro_image.h>

class BackGround
{
public:
	float x;
	float y;
	float velX;
	float velY;
	int dirX;
	int dirY;

	int WIDTH;
	int HEIGHT;

	ALLEGRO_BITMAP *image;
	BackGround();
	~BackGround();
};

