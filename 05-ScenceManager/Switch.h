#pragma once
#pragma once
#include "GameObject.h"
#include "Brick.h"

#define SWITCH_STATE_HIDEN				0
#define EFFECT_STATE					1
#define SWITCH_STATE_ACTIVE				2

#define SWITCH_ANI_NORMAL				0
#define SWITCH_ANI_ACTIVE				1

#define SWITCH_BBOX_WIDTH				16
#define SWITCH_BBOX_HEIGHT				16
#define SWITCH_ACTIVE_BBOX_HEIGHT		8

#define SWICH_RESET_TIME				5000

#define SWITCH_BRICK_AMOUNT				19



class CSwitch : public CGameObject
{
	CBrick* brick[SWITCH_BRICK_AMOUNT];
	DWORD switch_time;
public:
	CSwitch(CBrick* b[SWITCH_BRICK_AMOUNT]);
	void SetState(int state);
	void SetSwitch();
	void ResetItem();
	void SetSwitchTime(DWORD t) { switch_time = t; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	
};

