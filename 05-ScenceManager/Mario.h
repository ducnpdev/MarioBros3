#pragma once
#include "MarioConfig.h"
#include "Collision.h"
#include "GameObject.h"
#include "Coin.h" 


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

	DWORD timeStartKick;
	DWORD timeWalkingRight;
	DWORD timeWalkingLeft;
	DWORD timeMarioJumpStart;
	float marioSpeechJump;

	CGameObject* tortoiseshell;

public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

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
		return false;
	}

	// Function Get - Set
	bool GetMarioIsDead() { return marioIsDead; }
	void SetMarioIsDead(bool isDead) { marioIsDead = isDead; }

	int GetMarioLevel() { return level; }
	void SetMarioLevel(int le) { level = le; }

	bool GetMarioIsStateSitDown() { return isStateSitDown; }
	void SetMarioIsStateSitDown(bool stateSitDown) { isStateSitDown = stateSitDown; }

	bool GetMarioIsStandingFloor() { return isStandingFloor; }
	void SetMarioIsStandingFloor(bool stading) { isStandingFloor = stading; }

	int GetMarioIsJump() { return isJump; }
	void SetMarioIsJump(int i) { isJump = i; }

	void SetTimeJumpStart(DWORD t) { timeMarioJumpStart = t; }
	DWORD GetTimeJumpStart() { return timeMarioJumpStart; }

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

	bool GetMarioIsTortoiseshell() { return marioStateTorToiSeShell; }
	void SetMarioIsTortoiseshell(bool stateTortoiseshell) { marioStateTorToiSeShell = stateTortoiseshell; }

	void SetTimeStartKick(DWORD t) { timeStartKick = t; }
	DWORD GetTimeStartKick() { return timeStartKick; }

	void handleMarioTorToiSeShell();
	void marioSetUpDownLevel(int level);

	// object 
	void SetCoinHubPlay(CCoinPlay* c) { coinplayhub = c; }
	CCoinPlay* GetCoinHubPlay() { return coinplayhub; }
};
