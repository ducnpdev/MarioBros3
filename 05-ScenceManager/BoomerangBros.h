#pragma once
#include "Collision.h"
#include "PlayScence.h"
#include "GameObject.h"
#include "Boomerang.h"
#include "BoomerangConfig.h"

class CBoomerangBro : public CCollision
{
	CBoomerang* boomerang[BOOMERANG_AMOUNT];
	DWORD timeBoomerangBrosStart;
	bool foundMario;
	bool isBoomerangBrosIdle;
	bool isBoomerangBrosBack;
	bool isBoomerangBrosShoot;
	bool isBoomerangBrosRight;

	DWORD timeBoomerangBrosDead;
	bool isBoomerangBrosGoBack;

public:
	CBoomerangBro(CBoomerang* boomerang[BOOMERANG_AMOUNT]);
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	void handleDead();
	void SetBoomerangTimeDead(DWORD t) { timeBoomerangBrosDead = t; }
	
};