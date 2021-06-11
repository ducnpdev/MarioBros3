#include "Map1.h"

void CMap1::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void CMap1::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CMap1::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}
