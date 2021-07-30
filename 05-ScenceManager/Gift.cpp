#include "Gift.h"
#include "Utils.h"
#include "GameObject.h"

void CGift::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// if (state == GIFT_CLOSE) return;
	CGameObject::Update(dt, coObjects);
	if (itemStartOpen != 0 &&  GetTickCount64() - itemStartOpen > 100) {
		if (giftItem->GetState() != STATE_GIFT_ITEM_HIDEN) {

		giftItem->SetState(STATE_GIFT_ITEM_FLY);
		giftItem->SetTimePreHide((DWORD)GetTickCount64());
		}
	}
}

void CGift::Render()
{
	int ani = 0;
	if (state == GIFT_OPEN) ani = 1;
    animation_set->at(ani)->Render(x, y);
}

CGift::CGift(CGiftItem* _giftItem)
{
	giftItem = _giftItem;
	SetState(GIFT_CLOSE);
	 itemStartOpen = 0;
}

void CGift::SetState(int state)
{
	CGameObject::SetState(state);
}
void CGift::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GIFT_BBOX_WIDTH;
	bottom = y + GIFT_BBOX_HEIGHT;
}