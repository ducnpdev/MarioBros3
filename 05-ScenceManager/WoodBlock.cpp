#include "WoodBlock.h"

void CWoodBlock::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CWoodBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WOOD_BLOCK_BBOX_WIDTH;
	b = y + WOOD_BLOCK_BBOX_HEIGHT;
}
