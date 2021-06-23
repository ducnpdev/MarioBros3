#include "BlueBrick.h"

void CBlueBrick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CBlueBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLUE_BRICK_BBOX_WIDTH;
	b = y + BLUE_BRICK_BBOX_HEIGHT;
}