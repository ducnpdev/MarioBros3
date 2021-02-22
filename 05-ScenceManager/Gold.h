#pragma once
#include "GameObject.h"

class CGold : CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	CGold();
	virtual void SetState(int state);
};