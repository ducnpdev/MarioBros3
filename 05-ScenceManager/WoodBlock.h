#pragma once
#include "GameObject.h"
#include "Mushroom.h"
#include "Leaf.h"


#define WOOD_BLOCK_BBOX_WIDTH 16
#define WOOD_BLOCK_BBOX_HEIGHT 16
class CWoodBlock : public CGameObject
{
	CMushroom* mushroom;
	CLeaf* leaf;

public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
