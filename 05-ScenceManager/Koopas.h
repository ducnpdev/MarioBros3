
#pragma once
#include "GameObject.h"
#include "Collision.h"


#define KOOPAS_TIME_FLY 700

// speed
#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_SPINNING_SPEED 0.25f;
#define KOOPA_GRAVITY									0.001f
#define KOOPAS_FLY_INTERVAL 0.3f;
// B box
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_WIDTH_MIN 13
#define KOOPAS_BBOX_WIDTH_MEDIUM 15

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
#define KOOPAS_STATE_TAKEN 2000

// animation
#define KOOPAS_ANI_RED_WALKING_RIGHT 1
#define KOOPAS_ANI_RED_WALKING_LEFT 0
#define KOOPAS_ANI_RED_TORTOISESHELL_DOWN 2
#define KOOPAS_ANI_RED_SPIN_DOWN 3
#define KOOPA_ANI_RED_TAKEN_DOWN						4

#define KOOPA_ANI_GREEN_WALKING_RIGHT					5

#define KOOPA_ANI_GREEN_WALKING_LEFT					6
#define KOOPA_ANI_GREEN_TORTOISESHELL_DOWN				7
#define KOOPA_ANI_GREEN_SPIN_DOWN						8
#define KOOPAS_ANI_RED_TORTOISESHELL_UP				14
#define KOOPA_ANI_GREEN_TORTOISESHELL_UP			15
#define KOOPA_ANI_RED_REBORN_DOWN						16
#define KOOPA_ANI_GREEN_REBORN_DOWN						17
#define KOOPA_ANI_GREEN_TAKEN_UP						15

#define PARAKOOPA_ANI_GREEN_JUMPING_RIGHT				10
#define PARAKOOPA_ANI_GREEN_JUMPING_LEFT				11
// type koopa

#define KOOPA_COLOR_RED									0
#define KOOPA_GREEN_FORM								1

#define PARAKOOPA_COLOR_GREEN						2	
#define PARAKOOPA_TIME_DEFECT				300
// 

class CKoopas : public CCollision
{
	bool isKoopaDie = false;
	int ani = -1;
	int typeKoopa; 
	DWORD timeFly;
	DWORD timeStateTorToiSeShell;
	bool hidenStateKoopas = false;
	bool stateKoopaTortoiSeShell = false;

	float prePosY;

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
		
	void RedirectY();
	bool isDown = true;
	int stateDeflect = false; // after colission with tail
	DWORD timeStartDeflect;

public:
	CKoopas(int typeKoopa);
	virtual void SetState(int state);
	void SetTypeKoopa(int f) { typeKoopa = f; }
	int GetTypeKoopa() { return typeKoopa; }

	int RenderKoopaRed();
	int RenderKoopaGreen();
	int RenderParakoopaGreen();
	void SetStateDefect(bool is) { stateDeflect = is; }
	void SetDefectStart(DWORD time) { timeStartDeflect = time; }

	bool getIsDown() { return isDown; }
	void handlerDeflect();
};