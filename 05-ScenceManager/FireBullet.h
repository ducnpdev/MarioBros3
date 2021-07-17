#pragma once
#include "GameObject.h"
#include "Collision.h"

#define FIREBULLET_BBOX_WIDTH 8
#define FIREBULLET_BBOX_HEIGHT 8

#define FIRE_BULLET_FLYING_SPEECH			0.2f
#define FIRE_BULLET_DEFLECT_SPEED			0.2f
#define FIRE_BULLET_GRAVITY					0.001f

#define FIRE_BULLET_DESTROY_STATE			100
#define FIRE_BULLET_SHOOTED_RIGHT_STATE		200
#define FIRE_BULLET_SHOOTED_LEFT_STATE		300
#define FIRE_BULLET_TRANSPARENT_STATE		400

#define FIRE_BULLET_SHOOT_TIME				1000
#define FIRE_BULLET_DESTROY_TIME			1000

#define FIRE_BULLET_ANI_NORMAL		0
#define FIRE_BULLET_ANI_DESTROY		1

class CFireBullet : public CCollision
{
	DWORD shootStart;
	DWORD destroyStart;
public:
	CFireBullet();
	void SetShootStart(DWORD t) { shootStart = t; };
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

