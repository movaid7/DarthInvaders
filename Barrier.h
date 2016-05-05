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
	

	const int BHeight = 31;
	const int Bright = 100;
	const int Bleft  = 0;


};

static const char *Images[5] = { "Pictures/B4.png","Pictures/B3.png","Pictures/B2.png","Pictures/B1.png","Pictures/B0.png" };