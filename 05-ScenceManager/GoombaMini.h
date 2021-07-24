#pragma once
#pragma once
#include "GameObject.h"
#include "GoombaConfig.h"
#include "Collision.h"

class CGoombaMini : public CCollision
{
	DWORD timeDirection;
	bool checkCollisionMario = false;
public:
	CGoombaMini();
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
	void handleCollisionMario();
	void handleChangeDirection();
	float getMarioLevelNumber();
	bool GetCheckCollisionMario() { return checkCollisionMario; }
};

