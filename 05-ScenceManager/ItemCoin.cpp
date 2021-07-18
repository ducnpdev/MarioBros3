#include "ItemCoin.h"
#include "Utils.h"
#include "MarioConfig.h"

CCoin::CCoin(int _state)
{
	SetState(_state);
	listScore = NULL;
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case 3:
	//	if (!stateMoving) {
			stateMoving = true;
			timeStateMoving = GetTickCount64();
	//	}
		break;
	case COIN_STATE_MOVING:
		if (!stateMoving) {
			stateMoving = true;
			timeStateMoving = GetTickCount64();
		}
		break;
	case COIN_STATE_HIDEN:
		stateMoving = false;
		break;
	case COIN_STATE_NORMAL:
		stateMoving = false;
		break;
	default:
		break;
	}
}

void CCoin::Render()
{
	if (GetState() == COIN_STATE_HIDEN) return;

	int ani = COIN_EFFECT_ANI;
	if (GetState() == COIN_STATE_NORMAL)
		ani = COIN_NORMAL_ANI;
	if (GetState() == 3)
		ani = COIN_NORMAL_ANI;
	animation_set->at(ani)->Render(x, y);
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == COIN_STATE_HIDEN) return;
	CGameObject::Update(dt, coObjects);
	if (state == COIN_STATE_MOVING) {
		if (GetTickCount64() - timeStateMoving < 500) {
			vy = 0.1f;
			if (GetTickCount64() - timeStateMoving < 300) {
				vy = -0.1f;
			}
			// else vy = COIN_SPEED_Y;
			y += dy;
		}
		else {
			if (stateMoving)
			{
				SetState(COIN_STATE_HIDEN);
			}
			// TODO 
			//listScore->DisplayScores(MARIO_SCORE_100, x, y, GetTickCount64());
		}
	}
	if (state == 3) {
		if (GetTickCount64() - timeStateMoving < 500) {
			vy = 0.15f;
			if (GetTickCount64() - timeStateMoving < 300) {
				vy = -0.15f;
			}
			// else vy = COIN_SPEED_Y;
			y += dy;
		}
		else {
			SetState(COIN_STATE_HIDEN);
		}
	}
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
	if (state == COIN_STATE_HIDEN)
	{
		r = 0;
		b = 0;
	}
}


