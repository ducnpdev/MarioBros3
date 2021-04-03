
#pragma once
#include "GameObject.h"
#include "Collision.h"

// speed
#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_SPINNING_SPEED 0.25f;
#define KOOPA_GRAVITY									0.001f

// B box
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

// state
#define KOOPAS_STATE_WALKING_RIGHT						100
#define KOOPAS_STATE_WALKING_LEFT						200
#define KOOPAS_STATE_TORTOISESHELL_DOWN 300
#define KOOPAS_STATE_TORTOISESHELL_UP  400
#define KOOPAS_STATE_SPIN_RIGHT							500
#define KOOPAS_STATE_SPIN_LEFT							600

#define KOOPAS_STATE_REBORN 900
#define KOOPAS_STATE_DIE									1000
#define KOOPAS_STATE_HIDEN 1100

// animation
#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_RED_TORTOISESHELL_DOWN 2
#define KOOPAS_ANI_RED_SPIN_DOWN 3



class CKoopas : public CCollision
{
	DWORD timeStateTorToiSeShell;
	bool hidenStateKoopas = false;
	bool stateKoopaTortoiSeShell = false;

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

public:
	CKoopas();
	virtual void SetState(int state);
};