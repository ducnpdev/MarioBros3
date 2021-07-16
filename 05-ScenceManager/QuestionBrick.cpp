#include "QuestionBrick.h"
#include "Utils.h"
#include "ItemCoin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Switch.h"


CQuestionBrick::CQuestionBrick(float _originY)
{
	// SetState(QUESTIONBLOCK_ITEM_STATE);
	for (int i = 0; i < NUMBER_ITEM_IN_BRICK; i++) {
		itemInBrick[i] = NULL;
	}
	brickOriginY = _originY;
}

void CQuestionBrick::Render()
{
	int ani = 0;
	if (state == QUESTIONBLOCK_ITEM_STATE) {
		ani =  QUESTION_BRICK_ANI_MOVING;
	}
	else {
		 ani = QUESTION_BRICK_ANI_CRETE;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	// if (state != QUESTION_BRICK_STATE_CRETE) return;
	/*if (state != QUESTION_BRICK_STATE_CRETE) {
		y += dy;
		if (y < brickOriginY - 16) {
			SetState(QUESTION_BRICK_FALL);
		}
	}
	if (y > brickOriginY) y = brickOriginY;*/

	if (state != QUESTIONBLOCK_ITEM_STATE)
	{
		//DebugOut(L" sdfasd \n");
		if (state == QUESTIONBLOCK_DEFLECT_STATE)
		{
			if (y > brickOriginY - AFTER_COLLISION_MARIO) y += dy;
			else
			{
				SetState(QUESTION_BRICK_FALL);
				if (y < brickOriginY) {
					y = brickOriginY;
					if (!CheckQuestionBrickItem()) {
						SetState(QUESTIONBLOCK_NORMAL_STATE);
					}
					else {
						SetState(QUESTIONBLOCK_ITEM_STATE);
					}
				}
				else y += dy;
			}

		}
		else SetState(QUESTIONBLOCK_NORMAL_STATE);
	}
}

 

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;

	r = x + QUESTION_BRICK_BBOX_WIDTH;
	b = y + QUESTION_BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	/*case QUESTION_BRICK_STATE_MOVING:
		vx = 0;
		break;
	case QUESTION_BRICK_STATE_CRETE:
		vy = -0.125f;
		break;*/
	/*case QUESTION_BRICK_FALL:
		vy = 0.125f;
		break;*/
	case QUESTIONBLOCK_ITEM_STATE:
		vy = 0;
		break;
	case QUESTIONBLOCK_DEFLECT_STATE:
		vy = -0.125f;
		break;

	/*case QUESTIONBLOCK_ITEM_STATE:
		vy = 0;
		break;
	case QUESTIONBLOCK_DEFLECT_STATE:
		vy = -QUESTIONBLOCK_DEFLECT_SPEECH;
		break;*/
	case QUESTIONBLOCK_FALL_STATE:
		vy = 0.125f;
		break;
	case QUESTIONBLOCK_NORMAL_STATE:
		vy = 0;
		break;
	default:
		break;
	}
}

void CQuestionBrick::SetItemWhenCollision(int state)
{
	for (int i = 0; i < NUMBER_ITEM_IN_BRICK; i++)
	{
		if (itemInBrick[i] != NULL ) {
			if (itemInBrick[i]->GetState() == LEAF_STATE_HIDEN &&  dynamic_cast<CLeaf*>(itemInBrick[i])) {
			//	DebugOut(L"Question brick is Cleaf");
				itemInBrick[i]->SetState(LEAF_STATE_FLY);
				itemInBrick[i] = NULL;
				break;
			}
			if (itemInBrick[i]->GetState() == MUSHROOM_STATE_HIDEN && dynamic_cast<CMushroom*>(itemInBrick[i])) {
				itemInBrick[i]->SetPosition(x, y - 16);
				itemInBrick[i]->SetState(MUSHROOM_STATE_MOVING);
				itemInBrick[i] = NULL;
				break;
			}
			if (itemInBrick[i]->GetState() == COIN_STATE_HIDEN && dynamic_cast<CCoin*>(itemInBrick[i])) {	
				//DebugOut(L"Question brick is CCoin");
				itemInBrick[i]->SetPosition(x + 4, y - 16);
				itemInBrick[i]->SetState(state);
				itemInBrick[i] = NULL;
				break;
			}
			 if (itemInBrick[i]->GetState() == SWITCH_STATE_HIDEN && dynamic_cast<CSwitch*>(itemInBrick[i])){
			//	 DebugOut(L"Question brick is CSwitch");
				 itemInBrick[i]->SetPosition(x, y - 16);
				 itemInBrick[i]->SetState(EFFECT_STATE);
				 itemInBrick[i] = NULL;
				 break;
			}
		}

	}
}

void CQuestionBrick::PushItemQuestionBrick(CGameObject* tempItem, int countItem)
{
	for (int i = 0; i < NUMBER_ITEM_IN_BRICK; i++)
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

bool CQuestionBrick::CheckQuestionBrickItem()
{
	for (int i = 0; i < NUMBER_ITEM_IN_BRICK; i++)
	{
		if (itemInBrick[i] != NULL) return true;
	}
	return false;
}
