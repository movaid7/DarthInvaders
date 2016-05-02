#pragma once
#include<allegro5\allegro.h>
#include<allegro5\allegro_image.h>
using namespace std;
class Barrier
{

public:

	Barrier();
	void SetBarrierpos(int x, int y);
	void setLife(int l);
	void drawIfActive(float x, float y);

	int x_pos;
	int y_pos;
	int life_points;
	bool active;
	ALLEGRO_BITMAP *CurrentImage;
	const char *Images[5] = { "B4.png","B3.png","B2.png","B1.png","B0.png" };

	int BHeight = 31;
	int Bright = 100;
	int Bleft  = 0;


};