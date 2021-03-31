#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH				12
#define COIN_BBOX_HEIGHT			12

#define COIN_SPEED_Y				0.2f

// khoi tao thi hien tai la hiden
#define COIN_STATE_HIDEN			0

// sau khi mario collision brick
#define STATE_MOVING				1

// trang thai dung yen tai cho
#define COIN_STATE_NORMAL			2

#define COIN_NORMAL_ANI				0
#define COIN_EFFECT_ANI				1

#define COIN_EFFECT_TIME			500
#define COIN_EFFECT_MOVE_TIME		300

class CCoin : public CGameObject
{
	bool stateMoving;
	DWORD timeStateMoving;
public:
	CCoin(int _state);
	virtual void SetState(int state);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);	
};