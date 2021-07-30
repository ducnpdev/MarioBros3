#pragma once
#include "GameObject.h"
#include "Collision.h"

#define STATE_UP_VIP_ITEM_NORMAL	100
#define STATE_UP_VIP_ITEM_FLY	200
#define STATE_UP_VIP_ITEM_HIDEN	300
#define TIME_UP_VIP_ITEM_FLY_UP	700
#define TIME_UP_VIP_ITEM_FLY_DOWN 1500
#define TIME_UP_VIP_ITEM_FLY_VX 0.065f
#define TIME_UP_VIP_ITEM_FLY_VY 0.095f

class CUpVip : public CCollision
{
	DWORD timePreHiden = 0;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	void SetTimePreHide(DWORD time) { timePreHiden = time; }
	CUpVip();
	virtual void SetState(int state);
};