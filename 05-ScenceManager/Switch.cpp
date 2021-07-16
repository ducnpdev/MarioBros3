#include "Switch.h"
#include "ItemCoin.h"

CSwitch::CSwitch(CBrick* b[SWITCH_BRICK_AMOUNT])
{
	switch_time = 0;
	for (int i = 0; i < SWITCH_BRICK_AMOUNT; i++)
	{
		brick[i] = b[i];
	}
	SetState(SWITCH_STATE_HIDEN);

}

void CSwitch::SetState(int state)
{
	CGameObject::SetState(state);
}

void CSwitch::SetSwitch()
{
	for (int i = 0; i < SWITCH_BRICK_AMOUNT - 1; i++)
	{
		if (brick[i]->GetState() == BRICK_STATE_NORMAL)
		{
			brick[i]->SetState(BRICK_STATE_HIDEN);
			brick[i]->SetItem();
		}

	}
}

void CSwitch::ResetItem()
{
	for (int i = 0; i < SWITCH_BRICK_AMOUNT - 1; i++)
	{
		if (brick[i]->GetItem()->GetState() == COIN_STATE_NORMAL)
		{
			brick[i]->SetState(BRICK_STATE_NORMAL);
			brick[i]->GetItem()->SetState(COIN_STATE_HIDEN);
		}
	}
}

void CSwitch::Render()
{
	int ani;
	if (state == SWITCH_STATE_HIDEN) return;
	else if (state == EFFECT_STATE) ani = SWITCH_ANI_NORMAL;
	else ani = SWITCH_ANI_ACTIVE;
	animation_set->at(ani)->Render(x, y);
}

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (state == SWITCH_STATE_ACTIVE)
	{
		if (GetTickCount64() - switch_time > SWICH_RESET_TIME) {
		
			ResetItem();
		}
	}
}

void CSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == SWITCH_STATE_HIDEN) return;
	l = x;
	t = y;
	r = x + SWITCH_BBOX_WIDTH;
	b = y + SWITCH_BBOX_HEIGHT;
	if (state == SWITCH_STATE_ACTIVE)
	{
		b = y + SWITCH_ACTIVE_BBOX_HEIGHT;
	}

}
