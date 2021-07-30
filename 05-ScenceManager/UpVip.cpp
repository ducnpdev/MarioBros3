#include "UpVip.h"
#include "Utils.h"
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"

CUpVip::CUpVip()
{
	SetState(STATE_UP_VIP_ITEM_HIDEN);
}

void CUpVip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CUpVip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// DebugOut(L"state %d \n", state);

	if (state == STATE_UP_VIP_ITEM_NORMAL) {
		vx = 0.0f;
		vy = 0.0f;
		return;
	}
	if (state == STATE_UP_VIP_ITEM_HIDEN) {
		vx = 0.0f;
		vy = 0.0f;
		return;
	}

	if (state == STATE_UP_VIP_ITEM_FLY) {
		if (GetTickCount64() - timePreHiden < TIME_UP_VIP_ITEM_FLY_UP) {
			vx = -TIME_UP_VIP_ITEM_FLY_VX;
			vy = -TIME_UP_VIP_ITEM_FLY_VY;
			// DebugOut(L"1111 \n");
		}
		else if (GetTickCount64() - timePreHiden < TIME_UP_VIP_ITEM_FLY_DOWN) {
			//	DebugOut(L"222 \n");
			vx = -TIME_UP_VIP_ITEM_FLY_VX;
			vy = TIME_UP_VIP_ITEM_FLY_VY;
		}
		else {
			SetState(STATE_UP_VIP_ITEM_HIDEN);
			//	DebugOut(L"3333 \n");
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->GetMarioLive()->AddLives();
		}
	}
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}

void CUpVip::Render()
{
//	DebugOut(L"state %d \n", state);
	if (state == STATE_UP_VIP_ITEM_HIDEN) return;
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CUpVip::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_UP_VIP_ITEM_NORMAL:
		break;
	case STATE_UP_VIP_ITEM_FLY:
		break;
	default:
		break;
	}
}
