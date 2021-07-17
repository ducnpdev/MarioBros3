#pragma once
#include "GameObject.h"
#include "Mushroom.h"
#include "Leaf.h"


#define WOOD_BLOCK_BBOX_WIDTH 16
#define WOOD_BLOCK_BBOX_HEIGHT 16

#define WOOD_BLOCK_EFFECT_MARIO 1
#define WOOD_BLOCK_MOVING_LEFT 110
#define WOOD_BLOCK_MOVING_RIGHT 120
#define WOOD_BLOCK_TOP_LEFT 130
#define WOOD_BLOCK_TOP_RIGHT 140


class CWoodBlock : public CCollision
{
	CMushroom* mushroom;
	CLeaf* leaf;
	int type;
	bool isActive = false;
	float woodBlockOriginX;
	
	/*bool moving = false;
	DWORD timeMoving;*/
public:
	//void SetTimeMoving(DWORD t) { timeMoving = t; }
	CWoodBlock(int type, CMushroom* mushroom, CLeaf* leaf, float _originX);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* objects);
	virtual void SetState(int state);

	int GetWoodBlockType() { return type; }
	bool GetWoodBlockActive() { return isActive; }
	void SetWoodBlockActive(bool ac) { isActive = ac; }
	//void SetMarioIsFight(int type) { type = stateFight; }

	void RenderItemUpLevelMario();

	//bool GetWoodBlockMoving() { return moving; }
	//void SetWoodBlockMoving(bool ac) { moving = ac; }
};
