#pragma once
#include "GameObject.h"
#include "Collision.h"


#define KOOPA_HAMMER_GRAVITY	0.001f
#define KOOPAS_HAMMER_WALKING_SPEED 0.03f;
#define KOOPAS_HAMMER_WALKING_SPEED 0.03f;

#define KOOPAS_HAMMER_BBOX_HEIGHT 24
#define KOOPAS_HAMMER_BBOX_WIDTH 18


#define KOOPAS_HAMMER_STATE_WALKING_RIGHT		100
#define KOOPAS_HAMMER_STATE_WALKING_LEFT	200
#define KOOPAS_HAMMER_STATE_HIDEN 1000

#define KOOPAS_HAMMER_ANI_WALKING_RIGHT 0
#define KOOPAS_HAMMER_ANI_WALKING_LEFT 1

class CKoopasHammer : public CCollision
{
	bool hiden = false;

	// bool isKoopaDie = false;
//	int ani = -1;
//	int typeKoopa;
	// DWORD timeFly;
	// DWORD timeStateTorToiSeShell;
	//bool stateKoopaTortoiSeShell = false;
	//float prePosY;

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

	//void RedirectY();
	//bool isDown = true;
	// int stateDeflect = false; // after colission with tail
//	DWORD timeStartDeflect;

public:
	CKoopasHammer(int typeKoopa);
	virtual void SetState(int state);
	/*void SetTypeKoopa(int f) { typeKoopa = f; }
	int GetTypeKoopa() { return typeKoopa; }
	
	void SetStateDefect(bool is) { stateDeflect = is; }
	void SetDefectStart(DWORD time) { timeStartDeflect = time; }

	bool getIsDown() { return isDown; }
	int RenderKoopaRed();
	int RenderKoopaGreen();
	int RenderParakoopaGreen();
	void handlerDeflect();*/
};