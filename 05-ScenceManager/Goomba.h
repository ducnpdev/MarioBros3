#pragma once
#include "GameObject.h"
#include "Collision.h"

class CGoomba : public CCollision
{
	bool isDestroy = false;
	int jump_state;
	int hiden_state;
	int intro_state;
	int create_time;
	DWORD time_start;
	DWORD timeDestroy = 0;
public:
	CGoomba();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};