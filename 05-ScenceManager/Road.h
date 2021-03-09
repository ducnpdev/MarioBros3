#pragma once
#include "GameObject.h"

class CRoad : public CGameObject
{
	int type;
public:
	CRoad(int t) { type = t; }
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
