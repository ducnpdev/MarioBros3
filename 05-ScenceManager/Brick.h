#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define PIECE_BBOX_WIDTH	7
#define PIECE_BBOX_HEIGHT	7

#define BRICK_STATE_NORMAL	0
#define BRICK_STATE_HIDEN	1
#define BRICK_STATE_DESTROY	2
#define BRICK_ITEM_COIN				0
#define BRICK_ITEM_QUESTIONBLOCK	1
#define ITEM_AMOUNT		10
#define DISTANCE_BRICK_WITH_MARIO_MAX		20
#define DISTANCE_BRICK_WITH_MARIO_MIN		5
#define PIECE_BRICK_ANIMATIONSET	440

class CBrick : public CGameObject
{
	CGameObject* item[ITEM_AMOUNT];

public:
	CBrick();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
};
