#include "GiftItem.h"
#include "Utils.h"
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"

CGiftItem::CGiftItem()
{
	SetState(STATE_GIFT_ITEM_NORMAL);
}

void CGiftItem::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void CGiftItem::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == STATE_GIFT_ITEM_NORMAL) {
		vx = 0.0f;
		vy = 0.0f;
		return;
	} 
	if (state == STATE_GIFT_ITEM_HIDEN) {
		vx = 0.0f;
		vy = 0.0f;
		return;
	}
	//DebugOut(L"STATE_GIFT_ITEM_FLY \n");

	if (state == STATE_GIFT_ITEM_FLY) {
		if (GetTickCount64() - timePreHiden < TIME_GIFT_ITEM_FLY_UP) {
			vx = -0.05f;

			vy = -0.05f;
			// DebugOut(L"1111 \n");

		}
		else if (GetTickCount64() - timePreHiden < TIME_GIFT_ITEM_FLY_DOWN) {
		//	DebugOut(L"222 \n");
			vx = -0.05f;
			vy = 0.08f;
		}
		else {
			SetState(STATE_GIFT_ITEM_HIDEN);
		//	DebugOut(L"3333 \n");
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			mario->GetMarioLive()->AddLives();
		}
	}

	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}

void CGiftItem::Render()
{
	if (state == STATE_GIFT_ITEM_HIDEN) return;
    animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CGiftItem::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_GIFT_ITEM_NORMAL:
		break;
	case STATE_GIFT_ITEM_FLY:
		break;
	default:
		break;
	}
}
