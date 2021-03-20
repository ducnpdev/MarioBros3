#include "Hub.h"

CHub::CHub()
{
	time = NULL; 
	camera = NULL;
	coin = NULL;	
	arrows = NULL;
	for (int i = 0; i < MaxShowCards; i++) {
		cards[i] = NULL;
	}
}

void CHub::Render()
{
	animation_set->at(0)->Render(x, y);
	// RenderBoundingBox();
}

void CHub::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CHub::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CHub::SetTimeHub(CTime* t)
{
	time = t;
}

void CHub::SetCoinHub(CCoinPlay* c)
{
	coin = c;
}

void CHub::SetArrowHub(CListArrow* a)
{
	arrows = a;
}

void CHub::SetCardHub(CCard* c[MaxShowCards])
{
	for (int i = 0; i < MaxShowCards; i++) {
		cards[i] = c[i];
	}
}

void CHub::SetCameraHub(CCamera* c)
{
	camera = c;
}
