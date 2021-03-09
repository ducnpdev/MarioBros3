#include "Road.h"

#include "Road.h"
#include "Utils.h"

void CRoad::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CRoad::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (type == 0)
	{
		l = x;
		t = y;
		r = x + 32;
		b = y + 37;
	}
	if (type == 1)
	{
		l = x;
		t = y;
		r = x + 16;
		b = y + 14;
	}



}
