#pragma once
#include "GameObject.h"
#include "Collision.h"

#define GOOMBA_DEFLECT		0.09f
#define GOOMBA_WALKING_SPEED		0.03f
#define GOONBA_JUMP_DEFLECT_SPEED	0.2f
#define GOONBA_JUMP_SPEED			0.08f

#define GOOMBA_BBOX_WIDTH			16
#define GOOMBA_BBOX_HEIGHT			15

#define PARA_GOOMBA_BBOX_WIDTH		20
#define PARA_GOOMBA_BBOX_HEIGHT		18

#define PARA_GOOMBA_JUMPING_BBOX_WIDTH		20
#define PARA_GOOMBA_JUMPING_BBOX_HEIGHT		24

#define GOOMBA_BBOX_HEIGHT_DIE		9

#define GOOMBA_YELLOW_COLOR			0
#define GOOMBA_BROWN_FORM			1
#define PARA_GOOMBA_BROWN		2

#define GOOMBA_STATE_IDLE			0
#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_DIE			200
#define GOOMBA_STATE_DEFLECT		300
#define GOOMBA_STATE_JUMPING		400
#define GOOMBA_STATE_HIDEN			500

#define GOOMBA_ANI_YELLOW_WALKING			0
#define GOOMBA_ANI_BROWN_WALKING			1
#define PARA_GOOMBA_ANI_BROWN_JUMPING		2
#define PARA_GOOMBA_ANI_BROWN_WALKING		3
#define GOOMBA_ANI_YELLOW_DIE				4
#define GOOMBA_ANI_BROWN_DIE				5
#define GOOMBA_ANI_YELLOW_IDLE				6
#define GOOMBA_ANI_DEFLECT					0

#define DELTA_DESTROY_TIME					15
#define GOOMBA_DEAD_X						-50
#define GOOMBA_DEAD_Y						50

#define GOOMBA_RANGE_RIGHT_ATTACK_PLAYER	18
#define GOOMBA_RANGE_LEFT_ATTACK_PLAYER		-1

#define GOOMBA_AUTO_DEAD_ZONE				25

#define GOOMBA_INTRO_STATE_HIDEN			6500
#define GOOMBA_INTRO_STATE_IDLE				10800
#define GOOMBA_INTRO_STATE_WALKING			11000

#define PARAGOOMBA_ANI_TIME					2000			
#define PARAGOOMBA_ANI_JUMP_TIME			500

#define GOOMBA_GRAVITY				0.0005f
#define PLUST_POSITION_Y 5

class CGoomba : public CCollision
{
	int typeColorGoomba; // goomba color yellow, brown
	bool isVisibleGoomba = false; 
	DWORD timeVisibleGoomba = 0;
	bool goomStateJump = false;
	DWORD timeParaGoomba;

public:
	CGoomba(int type);

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
};