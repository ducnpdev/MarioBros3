#pragma once
#include "Collision.h"

class CBoomerang : public CCollision
{
	DWORD shootStartBoomerangTime;
	bool isBoomerangFly;
public:
	CBoomerang();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void SetState(int state);
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

	void SetShootStartBoomerang(DWORD time) { shootStartBoomerangTime = time; };
};

