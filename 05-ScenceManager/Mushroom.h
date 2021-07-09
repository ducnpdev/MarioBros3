#pragma once
#include "GameObject.h"
#include "Collision.h"

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_SPEED_Y	0.015f
#define MUSHROOM_SPEED_X	0.035f
#define MUSHROOM_GRAVITY	0.0006f

#define MUSHROOM_STATE_HIDEN	100
#define MUSHROOM_STATE_MOVING		200
#define MUSHROOM_STATE_NORMAL	300

#define MUSHROOM_EFFECT_TIME		1010
#define MUSHROOM_EFFECT_MOVE_TIME	200

#define MUSHROOM_RED_ANI		0

#define MUSHROOM_TIME_START_MOVING	200
#define MUSHROOM_TIME_NORMAL	400



class CMushroom : public CCollision
{
	bool stateMovingMushroom = false;
	DWORD timeStateMovingMushroom;
public:
	CMushroom(int _state);
	void SetState(int state);
	bool MushroomMoving() { return stateMovingMushroom; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

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