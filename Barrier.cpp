#include "Barrier.h"

using namespace std;

void Barrier::SetBarrierpos(int x, int y)
{
	x_pos = x;
	y_pos = y;

}


void Barrier::setLife(int l)
{
	life_points = l;
}
Barrier::Barrier() 
{
	 life_points = 5;
	 BHeight = 31;
	 Bright = 100;
	 Bleft = 0;
	 active = true;
}