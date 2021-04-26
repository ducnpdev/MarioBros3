#pragma once
#include "GameObject.h"
#include "Card.h"

#define CARDTEXT_STATE_HIDEN		0
#define CARDTEXT_STATE_DISPLAY		1

class CCardText : public CGameObject
{
	CCard* card;
public:
	CCardText(CCard* c);
	void SetState(int state);
	virtual void Render();
	CCard* GetCard() { return card; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {}
};

