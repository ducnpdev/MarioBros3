#pragma once
#include "GameObject.h"
#include "Collision.h"

#define MUSIC_BBOX_WIDTH 16
#define MUSIC_BBOX_HEIGHT 16

#define MUSIC_FLYING_SPEECH			0.2f
#define MUSIC_DEFLECT_SPEED			0.2f
#define MUSIC_GRAVITY					0.001f


#define MUSIC_HIDEN_STATE			100
#define MUSIC_APPEAR_STATE			200
#define MUSIC_DESTROY_STATE			300
#define MUSIC_DOWN_STATE			400
#define MUSIC_UP_STATE				500

#define MUSIC_SHOOT_TIME			1000
#define MUSIC_DESTROY_TIME			1000

#define MUSIC_ANI_WHITE		0
#define MUSIC_ANI_RED		1

#define MUSIC_TYPE_WHITE	0
#define MUSIC_TYPE_RED		1

#define MUSIC_RANGE_POSITION_Y	12


class CMusic : public CCollision
{
	DWORD shootStart;
	DWORD destroyStart;
	int type;
	float originY;
public:
	CMusic(int type, int _status, float _originY);
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

