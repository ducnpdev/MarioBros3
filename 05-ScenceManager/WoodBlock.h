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
	int type;
	bool isActive = false;
public:
	CWoodBlock(int type, CMushroom* mushroom, CLeaf* leaf);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);

	int GetWoodBlockType() { return type; }
	bool GetWoodBlockActive() { return isActive; }
	void SetWoodBlockActive(bool ac) { isActive = ac; }
	//void SetMarioIsFight(int type) { type = stateFight; }
};
