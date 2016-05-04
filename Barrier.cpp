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
	 active = true;
}

void Barrier::drawIfActive(float x, float y)
{
	if (this->active == true)
		al_draw_bitmap(CurrentImage, x, y, 0);
}