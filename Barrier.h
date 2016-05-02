#pragma once
using namespace std;
class Barrier
{

public:

	Barrier();
	void SetBarrierpos(int x, int y);
	void setLife(int l);

	int x_pos;
	int y_pos;
	int life_points;
	bool active;

	int BHeight = 31;
	int Bright = 100;
	int Bleft  = 0;

};