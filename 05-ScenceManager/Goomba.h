#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "GoombaMini.h"
#include "GoombaConfig.h"

class CGoomba : public CCollision
{
	int typeColorGoomba; // goomba color yellow, brown
	bool isVisibleGoomba = false; 
	DWORD timeVisibleGoomba = 0;
	bool goomStateJump = false;
	DWORD timeParaGoomba;

	DWORD timeFlyGoomba;

	CGoombaMini* goombaMini[NUMBER_GOOMBA_MINI];

public:
	CGoomba(int type,int tempState, CGoombaMini* goombaMini[NUMBER_GOOMBA_MINI]);

	void setColorGoomba(int _typeGoomba) { typeColorGoomba = _typeGoomba; }
	int getColorGoomba() { return typeColorGoomba; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
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

	void handleGoombaJumpInterval();

	void handleGoombaHighSmallGoomba();
};