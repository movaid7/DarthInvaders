#pragma once
using namespace std;
class Barrier
{

public:

	Barrier();
	void SetBarrierpos(int x, int y);
	void SetBarrierBound(int x, int y, int h);
	void setLife(int l);

	int x_pos;
	int y_pos;
	int life_points;
	bool active;

	int BHeight;
	int Bright;
	int Bleft;

};