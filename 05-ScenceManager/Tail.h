#pragma once
#include "GameObject.h"
#include "Collision.h"

#define TAIL_BBOX_WIDTH 7
#define TAIL_BBOX_HEIGHT 16

#define TAIL_STATE_HIDEN	100
#define TAIL_STATE_FIGHT	200

#define TAIL_FIGHT_SPEED	0.001f

class CTail : public CCollision
{
	DWORD fight_start;
public:
	CTail()
	{
		fight_start = 0;
		state = TAIL_STATE_FIGHT;
	}
	void SetState(int state);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
};