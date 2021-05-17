#pragma once
#include "GameObject.h"

#define QUESTION_BRICK_STATE_MOVING		100
#define QUESTION_BRICK_STATE_CRETE		200

#define QUESTION_BRICK_ANI_MOVING		0
#define QUESTION_BRICK_ANI_CRETE		1
#define  QUESTION_BRICK_FALL 2
#define NUMBER_ITEM_IN_BRICK  10

class CQuestionBrick : public CGameObject
{
	// item la vi du nhu coin, mushroom, ..
//	CGameObject* itemInBrick[11];
	float brickOriginY;

	CGameObject* itemInBrick[NUMBER_ITEM_IN_BRICK];

public:
	CQuestionBrick(float _originY);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void SetItemWhenCollision(int state);
	void PushItemQuestionBrick(CGameObject* item, int _countItem);

	CGameObject* GetItemInBrick();

};
