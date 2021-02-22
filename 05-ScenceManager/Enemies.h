#pragma once
#include "GameObject.h"

class CEnemies : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
public:
	CEnemies();
	virtual void SetState(int state);
};