#pragma once
#include "GameObject.h"
#include "MarioConfig.h"

class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	int ani = -1;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	bool marioIsDead = false;
	bool isRunning = false;
	bool isJump = false;
	bool isSitDown = false;


public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	// Function Render 
	virtual void Render();
	int RenderLevelMarioSmall();
	int RenderLevelMarioBig();
	int RenderLevelMarioTail();
	int RenderLevelMarioFire();

	// Function Get - Set
	bool GetMarioIsDead() { return marioIsDead; }
	void SetMarioIsDead(bool isDead) { marioIsDead = isDead; }
	int GetMarioLevel() { return level; }
	void SetMarioLevel(int le) { level = le; }

};