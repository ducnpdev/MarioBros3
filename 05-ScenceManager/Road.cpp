#include "Road.h"

#include "Road.h"
#include "Utils.h"

void CRoad::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CRoad::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 14;
}

void CBorderRoad::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CBorderRoad::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}
