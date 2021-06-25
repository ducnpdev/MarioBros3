#pragma once
#include "MarioConfig.h"
#include "Collision.h"
#include "GameObject.h"
#include "Coin.h" 
#include "Tail.h" 
#include "ListScore.h"
#include "Arrow.h"
#include "Card.h"


class CMario : public  CCollision
{
	// Object
	CCoinPlay* coinplayhub; // item coin in hub

	int level;
	int untouchable;
	DWORD untouchable_start;
	int ani = -1;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	bool marioIsDead = false;
	bool isRunning = false;
	bool isStateSitDown = false;
	bool isStandingFloor = false;
	bool isTurn = false;
	bool isKick = false;
	int isJump = 0;
	bool marioStateFall = false;
	bool marioStateDie = false;
	bool marioStateTorToiSeShell = false;
	bool marioStateUpLevel = false;
	bool marioStateSmoke = false;
	bool isJumpFlyLow = false;
	bool marioStateFight = false;
	bool marioStateMaxPower = false;
	DWORD timeMarioFight;

	CTail* tail;
	DWORD timeStartKick;
	DWORD timeWalkingRight;
	DWORD timeWalkingLeft;
	DWORD timeMarioJumpStart;
	DWORD timeMarioJumpFlyLow;
	float marioSpeechJump;
	DWORD timeMarioUpLevel;
	DWORD timeMarioSmoke;

	bool marioStatePipeDown;
	bool  marioStatePipeUp;
	// int pipe_down_fast_state;
	DWORD  timeMarioPipeDown;
	DWORD  timeMarioPipeUp;
	// DWORD pipe_down_fast_start;

	CGameObject* tortoiseshell;
	// start hub
	CCoinPlay* coinplay;
	CListScore* listScore[3];
	CCard* card[MaxShowCards];
	// end hub
	DWORD timeRunningStart;
	CListArrow* listArrow;

	// fly hight
	DWORD timeMarioFlyHigh;
	bool marioStateFlyHigh = false;


public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);
	// Function Render 
	virtual void Render();
	int RenderLevelMarioSmall();
	int RenderLevelMarioBig();
	int RenderLevelMarioTail();
	int RenderLevelMarioFire();

	bool GetMarioPower(){
		//TODO word P in hud 
	//	return false;
		if (listArrow->GetPState()) return false;
		return true;
	}

	void SetListArrow(CListArrow* list) { listArrow = list; }
	CListArrow* GetListArrow() { return listArrow; }


	// Function Get - Set
	void SetCards(CCard* c[MaxShowCards]) {
		for (int i = 0; i < MaxShowCards; i++)
		{
			card[i] = c[i];
		}
	}
	void SetCardState(int s)
	{
		for (int i = 0; i < MaxShowCards; i++)
		{
			if (card[i]->GetState() == CARD_STATE_EMPTY) {
				card[i]->SetState(s);
				break;
			}
		}
	}
	CCard* GetCard() {
		return card[0]; 
	}

	DWORD GetMarioFlyHighStart() { return timeMarioFlyHigh; }
	void SetMarioFlyHighStart(DWORD t) { timeMarioFlyHigh = t; }
	bool GetMarioFlyHighState() { return marioStateFlyHigh; }

	bool GetMarioIsDead() { return marioIsDead; }
	void SetMarioIsDead(bool isDead) { marioIsDead = isDead; }

	int GetMarioLevel() { return level; }
	void SetMarioLevel(int le) { x = x - 10; level = le; }

	bool GetMarioIsStateSitDown() { return isStateSitDown; }
	void SetMarioIsStateSitDown(bool stateSitDown) { isStateSitDown = stateSitDown; }

	bool GetMarioIsStandingFloor() { return isStandingFloor; }
	void SetMarioIsStandingFloor(bool stading) { isStandingFloor = stading; }

	int GetMarioIsJump() { return isJump; }
	void SetMarioIsJump(int i) { isJump = i; }

	void SetTimeJumpStart(DWORD t) { timeMarioJumpStart = t; }
	DWORD GetTimeJumpStart() { return timeMarioJumpStart; }


	void SetTimeJumpStartFlyLow(DWORD t) { timeMarioJumpFlyLow = t; }
	DWORD GetTimeJumpStartFlyLow() { return timeMarioJumpFlyLow; }

	void SetTimeWalkingLeft(DWORD t) { timeWalkingLeft = t; }
	DWORD GetTimeWalkingLeft() { return timeWalkingLeft; }
		
	void SetTimeWalkingRight(DWORD t) { timeWalkingRight = t; }
	DWORD GetTimeWalkingRight() { return timeWalkingRight; }

	float GetMarioSpeechJump() { return marioSpeechJump; }
	void SetMarioSpeechJump() { marioSpeechJump += 0.00025f; }

	float GetMarioFallState() { return marioStateFall; }
	void SetMarioFallState(int _fallstate) { marioStateFall += _fallstate; }

	int GetBBoxWidthMario();
	int GetBBoxHeightMario();

	bool GetMarioIsDie() { return marioStateDie; }
	void SetMarioIsDie(bool stateDie) { marioStateDie = stateDie; }

	bool GetMarioIsFight() { return marioStateFight; }
	void SetMarioIsFight(bool stateFight) { marioStateFight = stateFight; }
	void SetTimeIsFight(DWORD t) { timeMarioFight = t; }
	DWORD GetTimeIsFight() { return timeMarioFight; }

	bool GetMarioIsTortoiseshell() { return marioStateTorToiSeShell; }
	void SetMarioIsTortoiseshell(bool stateTortoiseshell) { marioStateTorToiSeShell = stateTortoiseshell; }

	bool GetMarioIsUpLevel() { return marioStateUpLevel; }
	void SetMarioIsUpLevel(bool uplevel) { marioStateUpLevel = uplevel; }

	void SetTimeStartKick(DWORD t) { timeStartKick = t; }
	DWORD GetTimeStartKick() { return timeStartKick; }

	void marioSetUpDownLevel(int level);

	// object 
	void SetCoinHubPlay(CCoinPlay* c) { coinplayhub = c; }
	CCoinPlay* GetCoinHubPlay() { return coinplayhub; }
	void CreateListScore(CListScore* s);
	void DisplayListScore(int s, float x, float y, DWORD t);
	void CreateTail(CTail* t);


	// func sub handler
	void handleMarioTorToiSeShell();
	void handlerMarioJumpFly();
	void plustortoiseshellInMario(int numberPlusLeft, int numberPlusRight);
	void handlerMarioUpLevelOtherSmall();
	void handlerMarioUpLevelSmoke(); 
	void handlerMarioFight();
	void handlerMarioDownPipe();
	void handlerMarioUpPipe();
	// collision
	void MarioCollisionPiranhaPlant();
	void CollisionWithKoopa(LPCOLLISIONEVENT collisionEven);
	void CollisionWithGoomba(LPCOLLISIONEVENT collisionEven);
	void CollisionWithPipe(LPCOLLISIONEVENT collisionEven);
	// 
	void MarioHanlerProcessArrow();
	bool checkMarioMaxPower();

	void handleMarioDead();
	void handleMarioFlyHigh();

};
