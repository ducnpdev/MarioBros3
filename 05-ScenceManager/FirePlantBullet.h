#pragma once
#include "GameObject.h"
#include "Collision.h"

#define FIREPLANTBULLET_BBOX_WIDTH 8
#define FIREPLANTBULLET_BBOX_HEIGHT 8

#define FIRE_PLANT_BULLET_FLYING_SPEECH_X			0.08f
#define FIRE_PLANT_BULLET_FLYING_SPEECH_Y			0.08f


#define FIREPLANTBULLET_DESTROY_STATE				100
#define FIREPLANTBULLET_SHOOTED_RIGHT_30_STATE		200
#define FIREPLANTBULLET_SHOOTED_RIGHT_60_STATE		210
#define FIREPLANTBULLET_SHOOTED_RIGHT_120_STATE		220
#define FIREPLANTBULLET_SHOOTED_RIGHT_150_STATE		230
#define FIREPLANTBULLET_SHOOTED_LEFT_30_STATE		300
#define FIREPLANTBULLET_SHOOTED_LEFT_60_STATE		310
#define FIREPLANTBULLET_SHOOTED_LEFT_120_STATE		320
#define FIREPLANTBULLET_SHOOTED_LEFT_150_STATE		330
#define FIREPLANTBULLET_TRANSPARENT_STATE			400

#define FIREPLANTBULLET_DESTROY_TIME				2000
#define FIREPLANTBULLET_SPEED_PLUS_X				0.01f
#define FIREPLANTBULLET_SPEED_PLUS_Y				0.05f




class CFirePlantBullet : public CCollision
{
	DWORD shoot_start;
public:
	CFirePlantBullet();
	void SetShootStart(DWORD t);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	virtual void SetState(int s);
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