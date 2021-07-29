#pragma once
#include "GameObject.h"
#include "Collision.h"

#define STATE_GIFT_ITEM_NORMAL	100
#define STATE_GIFT_ITEM_FLY	200
#define STATE_GIFT_ITEM_HIDEN	300
#define TIME_GIFT_ITEM_FLY_UP	200
#define TIME_GIFT_ITEM_FLY_DOWN 1200

class CGiftItem : public CCollision
{
	DWORD timePreHiden = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	void SetTimePreHide(DWORD time) { timePreHiden = time; }
	CGiftItem();
	virtual void SetState(int state);
};