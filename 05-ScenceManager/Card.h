#pragma once
#include "GameObject.h"

class CCard : public CGameObject
{
public:
	virtual void SetState(int state);
	virtual void Render();	
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);
	virtual void GetBoundingBox(float& lelf, float& t, float& r, float& b);
};
