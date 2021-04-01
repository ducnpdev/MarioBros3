#include "ItemCoin.h"
#include "Utils.h"

CCoin::CCoin(int _state)
{
	SetState(COIN_STATE_HIDEN);
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_MOVING:
		if (!stateMoving) {
			stateMoving = true;
			timeStateMoving = GetTickCount();
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
//	DebugOut(L"CCoin upate %f \n", GetState());
	animation_set->at(1)->Render(x, y);
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == COIN_STATE_HIDEN) return;

	// DebugOut(L"state CCoin %d\n", state);
	CGameObject::Update(dt, coObjects);
	if (state == COIN_STATE_MOVING) {

		if (GetTickCount() - timeStateMoving < 500) {
			vy = 0.1f;
			if (GetTickCount() - timeStateMoving < 300) {
				vy = -0.1f;
			}
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
}
