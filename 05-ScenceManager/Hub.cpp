#include "Hub.h"

CHub::CHub() {
	// camera = NULL;
}

void CHub::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CHub::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}

void CHub::GetBoundingBox(float& l, float& t, float& r, float& b) { 
}
