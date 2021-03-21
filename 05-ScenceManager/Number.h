#pragma once
#include "GameObject.h"
#include "Utils.h"

#define NUMBER_STATE_0		0
#define NUMBER_STATE_1		1
#define NUMBER_STATE_2		2
#define NUMBER_STATE_3		3
#define NUMBER_STATE_4		4
#define NUMBER_STATE_5		5
#define NUMBER_STATE_6		6
#define NUMBER_STATE_7		7
#define NUMBER_STATE_8		8
#define NUMBER_STATE_9		9

#define NUMBER_ANI_0		0
#define NUMBER_ANI_1		1
#define NUMBER_ANI_2		2
#define NUMBER_ANI_3		3
#define NUMBER_ANI_4		4
#define NUMBER_ANI_5		5
#define NUMBER_ANI_6		6
#define NUMBER_ANI_7		7
#define NUMBER_ANI_8		8
#define NUMBER_ANI_9		9


class CNumber : public CGameObject
{
	int number;

public:
	CNumber() { number = 0; }
	void SetNumber(int num) { number = num; }
	int GetNumber() { return number; }
	void SetState(int state);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

