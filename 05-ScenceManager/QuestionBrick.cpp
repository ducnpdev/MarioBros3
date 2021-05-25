#include "QuestionBrick.h"
#include "Utils.h"
#include "ItemCoin.h"
#include "Mushroom.h"
#include "Leaf.h"


CQuestionBrick::CQuestionBrick(float _originY)
{
	SetState(QUESTION_BRICK_STATE_MOVING);
	for (int i = 0; i < 11; i++) {
		itemInBrick[i] = NULL;
	}
	brickOriginY = _originY;
}

void CQuestionBrick::Render()
{
	int ani = QUESTION_BRICK_ANI_MOVING;
	if (state != QUESTION_BRICK_STATE_MOVING) {
		ani = QUESTION_BRICK_ANI_CRETE;
	}
	animation_set->at(ani)->Render(x, y);
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	y += dy;
	if (state == QUESTION_BRICK_ANI_CRETE) {
		if (y < brickOriginY - 16) {
			SetState(QUESTION_BRICK_FALL);
		}
	}

	if (y > brickOriginY) y = brickOriginY;
}


void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}

void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BRICK_STATE_MOVING:
		vx = 0;
		break;
	case QUESTION_BRICK_ANI_CRETE:
		vy = -0.125f;
		break;
	case QUESTION_BRICK_FALL:
		vy = 0.125f;;
	default:
		break;
	}
}

void CQuestionBrick::SetItemWhenCollision(int state)
{
	for (int i = 0; i < 5; i++)
	{
		if (itemInBrick[i] != NULL ) {
			if (itemInBrick[i]->GetState() == LEAF_STATE_HIDEN &&  dynamic_cast<CLeaf*>(itemInBrick[i])) {
				itemInBrick[i]->SetState(state);
			}
			if (itemInBrick[i]->GetState() == MUSHROOM_STATE_HIDEN && dynamic_cast<CMushroom*>(itemInBrick[i])) {
				itemInBrick[i]->SetPosition(x, y - 16);
				itemInBrick[i]->SetState(MUSHROOM_STATE_MOVING);
			}
			if (itemInBrick[i]->GetState() == COIN_STATE_HIDEN && dynamic_cast<CCoin*>(itemInBrick[i])) {
				itemInBrick[i]->SetPosition(x + 4, y - 16);
				itemInBrick[i]->SetState(state);
			}
		}
	}
}

void CQuestionBrick::PushItemQuestionBrick(CGameObject* tempItem, int countItem)
{
	for (int i = 0; i < 10; i++)
	{
		if(itemInBrick[i] == NULL){
			itemInBrick[i] = tempItem;
			return;
		}
	}
}

CGameObject* CQuestionBrick::GetItemInBrick()
{
	for (int i = 0; i < NUMBER_ITEM_IN_BRICK; i++)
	{
		if (itemInBrick[i] != NULL) return itemInBrick[i];
	}
	return NULL;
}
