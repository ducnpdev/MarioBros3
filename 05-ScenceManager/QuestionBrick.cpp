#include "QuestionBrick.h"
#include "Utils.h"

#define QUESTION_BRICK_STATE_MOVING		100
#define QUESTION_BRICK_STATE_CRETE		200

#define QUESTION_BRICK_ANI_MOVING		0
#define QUESTION_BRICK_ANI_CRETE		1

CQuestionBlock::CQuestionBlock()
{
	SetState(QUESTION_BRICK_STATE_MOVING);
}

void CQuestionBlock::Render()
{
	int ani = QUESTION_BRICK_ANI_MOVING;
	if (state != QUESTION_BRICK_STATE_MOVING) {
		ani = QUESTION_BRICK_ANI_CRETE;
	}
	animation_set->at(ani)->Render(x, y);
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	DebugOut(L" class question brick \n");
}


void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}

void CQuestionBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BRICK_STATE_MOVING:
		vx = 0;
		break;
	default:
		break;
	}
}
