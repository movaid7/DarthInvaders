#include "Barrier.h"

using namespace std;

void Barrier::SetBarrierpos(int x, int y)
{
	x_pos = x;
	y_pos = y;

}

void Barrier::SetBarrierBound(int x, int y, int h)
{
	Bright = x;
	Bleft = y;
	BHeight = h;
}

void Barrier::setLife(int l)
{
	life_points = l;
}
Barrier::Barrier() {}