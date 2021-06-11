#pragma once
#include "GameObject.h"
class CMap1 : public CGameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};