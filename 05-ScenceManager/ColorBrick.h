#pragma once
#include "GameObject.h"

#define COLOR_BRICK_BBOX_HEIGHT 16
#define COLOR_BRICK_BBOX_WIDTH	16

class CColorBrick : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};