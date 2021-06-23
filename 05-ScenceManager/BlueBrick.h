#pragma once
#include "GameObject.h"

#define BLUE_BRICK_BBOX_WIDTH 16
#define BLUE_BRICK_BBOX_HEIGHT 16

class CBlueBrick : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

