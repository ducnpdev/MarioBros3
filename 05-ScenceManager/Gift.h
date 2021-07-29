#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "GiftItem.h"

#define GIFT_CLOSE 100
#define GIFT_OPEN	200

#define GIFT_BBOX_WIDTH 32
#define GIFT_BBOX_HEIGHT 32

class CGift : public CCollision
{
	CGiftItem* giftItem;
	DWORD itemStartOpen;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	void SetTimeStartOpen(DWORD time) { itemStartOpen = time; }
	CGift(CGiftItem* _giftItem);
	virtual void SetState(int state);
};